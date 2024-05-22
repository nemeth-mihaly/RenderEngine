#include "Application.h"

#include <cstring>
#include <format>
#include <iostream>

Application* g_pApp = nullptr;

static const wchar_t ClassName[] = TEXT("MyWindow");

// Window procedure wrapped in C++ class:
//      https://devblogs.microsoft.com/oldnewthing/20191014-00/?p=102992

LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Application* pApp = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pApp = static_cast<Application*>(lpcs->lpCreateParams);
        SetWindowLongPtr(hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));

        pApp->m_hWindow = hWindow;
    }
    else
    {
        pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWindow, GWLP_USERDATA));
    }

    if (pApp)
    {
        return pApp->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hWindow, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////
//  Application Implementation
////////////////////////////////////////////////////

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
    g_pApp = this;

    m_ScreenWidth = 1280;
    m_ScreenHeight = 720;

    m_hWindow = nullptr;

    memset(m_bKeyStates, 0, sizeof(m_bKeyStates));

    m_hDeviceContext = nullptr;
    m_hGLRenderContext = nullptr;

    m_bIsRunning = false;

    m_AssetManager = nullptr;

    m_bCameraMoving = false;
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;
}

Application::~Application()
{
    if (m_AssetManager)
    {
        delete m_AssetManager;
    }

    wglDeleteContext(m_hGLRenderContext);
    ReleaseDC(m_hWindow, m_hDeviceContext);

    DestroyWindow(m_hWindow);
}

bool Application::Initialize()
{
    InitWindow();
    InitOpenGL();
 
    ShowWindow(m_hWindow, SW_SHOW);
    SetFocus(m_hWindow);

    m_AssetManager = new AssetManager();

    m_AssetManager->LoadShaderProgram("TexturedLit", "Assets\\Shaders\\TexturedLit_vert.glsl", "Assets\\Shaders\\TexturedLit_frag.glsl");
    m_AssetManager->LoadShaderProgram("Sky", "Assets\\Shaders\\Sky_vert.glsl", "Assets\\Shaders\\Sky_frag.glsl");
    m_AssetManager->LoadShaderProgram("FramebufferTest", "Assets\\Shaders\\Framebuffer_test_vert.glsl", "Assets\\Shaders\\Framebuffer_test_frag.glsl");

    m_AssetManager->LoadWavefrontMesh("Assets\\Models\\Rectangle.obj");
    m_AssetManager->LoadWavefrontMesh("Assets\\Models\\Cube.obj");
    m_AssetManager->LoadWavefrontMesh("Assets\\Models\\Monkey.obj");
    m_AssetManager->LoadWavefrontMesh("Assets\\Models\\Grass.obj");

    m_AssetManager->LoadTexture("Assets\\Textures\\UvGrid.png");
    m_AssetManager->LoadTexture("Assets\\Textures\\Stonebricks.png");
    m_AssetManager->LoadTexture("Assets\\Textures\\Grass.png");
    m_AssetManager->LoadTexture("Assets\\Textures\\SphereGlow.png");
    m_AssetManager->LoadTexture("Assets\\Textures\\AlphaWindow.png");

    std::vector<std::string> skyTextureNames =
    {
        "Assets\\Textures\\Sky_PX.png",
        "Assets\\Textures\\Sky_NX.png",
        "Assets\\Textures\\Sky_PY.png",
        "Assets\\Textures\\Sky_NY.png",
        "Assets\\Textures\\Sky_PZ.png",
        "Assets\\Textures\\Sky_NZ.png",
    };

    stbi_set_flip_vertically_on_load(false);

    m_Texture_Sky.reset(new Texture());
    assert(m_Texture_Sky);
    m_Texture_Sky->Create(GL_TEXTURE_CUBE_MAP);

    m_Texture_Sky->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channelCount;
    stbi_load(skyTextureNames[0].c_str(), &width, &height, &channelCount, 0);

    for (uint32_t i = 0; i < skyTextureNames.size(); ++i)
    {
        uint8_t* pPixels = stbi_load(skyTextureNames[i].c_str(), &width, &height, &channelCount, 0);
        assert(pPixels);
    
        m_Texture_Sky->SetImage2DForSpecialTarget(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureInternalFormat(channelCount), width, height, 0, TextureFormat(channelCount), pPixels);
    }

    std::vector<Vertex> rectangleVertices = CreateRectangleVertices();
    m_Mesh_Rectangle.reset(new Mesh());
    assert(m_Mesh_Rectangle);
    m_Mesh_Rectangle->VertexCount = rectangleVertices.size();
    m_Mesh_Rectangle->pVertices = rectangleVertices.data();
    m_Mesh_Rectangle->VertexBufferOffset = 0;
    m_Mesh_Rectangle->Create();
    rectangleVertices.clear();

    std::vector<Vertex> cubeVertices = CreateCubeVertices();

    m_Mesh_Cube.reset(new Mesh());
    assert(m_Mesh_Cube);
    m_Mesh_Cube->VertexCount = cubeVertices.size();
    m_Mesh_Cube->pVertices = cubeVertices.data();
    m_Mesh_Cube->VertexBufferOffset = 0;
    m_Mesh_Cube->Create();
    cubeVertices.clear();

    // ----------------------------------------------------

    // ----------------------------------------------------

    m_SceneNodes.reserve(32);
    m_LightNodes.reserve(32);

    m_Camera.reset(new CameraNode());
    m_Camera->VCreate();
    m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_SceneNodes.push_back(m_Camera);

    auto floorNode = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Cube.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    floorNode->VCreate();
    floorNode->SetPosition(glm::vec3(0.0f, -0.55f, -0.0f));
    floorNode->SetScale(glm::vec3(10.0f, 0.1f, 10.0f));
    floorNode->GetMaterial().Diffuse = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
    floorNode->GetMaterial().Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    floorNode->GetMaterial().bUseTexture = false;

    auto node1 = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Cube.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    node1->VCreate();
    node1->SetPosition(glm::vec3(-1.5f, 0.0f, -2.5f));
    node1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node1->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    node1->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    node1->GetMaterial().bUseTexture = true;

    auto node2 = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Cube.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    node2->VCreate();
    node2->SetPosition(glm::vec3(2.0f, 0.0f, -2.5f));
    node2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node2->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    node2->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    node2->GetMaterial().bUseTexture = true;

    auto node3 = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Cube.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    node3->VCreate();
    node3->SetPosition(glm::vec3(1.0f, 0.0f, -3.5f));
    node3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node3->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    node3->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    node3->GetMaterial().bUseTexture = true;

    auto monkeyNode = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Monkey.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    monkeyNode->VCreate();
    monkeyNode->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    monkeyNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    monkeyNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    monkeyNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    monkeyNode->GetMaterial().bUseTexture = true;

    std::vector<glm::vec3> grassPositions =
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    for (uint32_t i = 0; i < grassPositions.size(); ++i)
    {
        auto grassNode = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Grass.obj", "TexturedLit", "Assets\\Textures\\Grass.png"));
        grassNode->VCreate();
        grassNode->SetPosition(grassPositions[i]);
        grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        grassNode->GetMaterial().bUseTexture = true; 
    }

    std::vector<glm::vec3> glowPositions =
    {
        glm::vec3(-1.5f, 0.3f, 0.48f),
        glm::vec3( 1.5f, 0.3f, 1.51f),
        glm::vec3( 0.0f, 0.3f, 1.7f),
        glm::vec3(-0.3f, 0.3f, 2.3f),
        glm::vec3(0.5f, 0.3f, 0.6f)
    };

    for (uint32_t i = 0; i < glowPositions.size(); ++i)
    {
        std::string textureName = "Assets\\Textures\\SphereGlow.png";

        if (i % 2 == 0)
        {
            textureName = "Assets\\Textures\\AlphaWindow.png";
        }

        auto grassNode = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Rectangle.obj", "TexturedLit", textureName));
        grassNode->VCreate();
        grassNode->SetPosition(glowPositions[i]);
        grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        grassNode->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
        grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        grassNode->GetMaterial().bUseTexture = true; 
    }

    // DirectionalLightNode
    LightProperties directionalLightProperties;
    directionalLightProperties.Type = LightType::Directional;
    directionalLightProperties.Direction = glm::vec3(1.2f, -1.0f, 1.3f);
    directionalLightProperties.Ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    directionalLightProperties.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    directionalLightProperties.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);;

    m_DirectionalLightNode.reset(new LightNode());
    m_DirectionalLightNode->VCreate();
    m_DirectionalLightNode->SetLightProperties(directionalLightProperties);
    m_SceneNodes.push_back(m_DirectionalLightNode);
    m_LightNodes.push_back(m_DirectionalLightNode);

    // PointLightNode
    LightProperties pointLightProperties;
    pointLightProperties.Type = LightType::Point;
    pointLightProperties.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    pointLightProperties.Ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pointLightProperties.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pointLightProperties.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);;
    pointLightProperties.ConstantAttenuation = 1.0f;
    pointLightProperties.LinearAttenuation = 0.09f;
    pointLightProperties.QuadraticAttenuation = 0.032f;

    m_PointLightNode.reset(new LightNode());
    m_PointLightNode->VCreate();
    m_PointLightNode->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));
    m_PointLightNode->SetLightProperties(pointLightProperties);
    m_SceneNodes.push_back(m_PointLightNode);
    m_LightNodes.push_back(m_PointLightNode);

    // SpotLightNode
    LightProperties spotLightProperties;
    spotLightProperties.Type = LightType::Spot;
    spotLightProperties.Position = glm::vec3(3.4f, 1.0f, 3.0f);
    spotLightProperties.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLightProperties.Ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0);
    spotLightProperties.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0);
    spotLightProperties.Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0);
    spotLightProperties.Falloff = cosf(glm::radians(30.0f));
    spotLightProperties.Phi = cosf(glm::radians(36.0f));
    spotLightProperties.ConstantAttenuation = 1.0f;
    spotLightProperties.LinearAttenuation = 0.09f;
    spotLightProperties.QuadraticAttenuation = 0.032f;

    m_SpotLightNode.reset(new LightNode());
    m_SpotLightNode->VCreate();
    m_SpotLightNode->SetPosition(glm::vec3(0.4f, 1.0f, -2.0f));
    m_SpotLightNode->SetLightProperties(spotLightProperties);
    m_SceneNodes.push_back(m_SpotLightNode);
    m_LightNodes.push_back(m_SpotLightNode);

    auto lightBulb = m_SceneNodes.emplace_back(new MeshNode("Assets\\Models\\Cube.obj", "TexturedLit", "Assets\\Textures\\UvGrid.png"));
    lightBulb->VCreate();
    lightBulb->SetPosition(pointLightProperties.Position);
    lightBulb->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    lightBulb->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightBulb->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lightBulb->GetMaterial().Emissive = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightBulb->GetMaterial().bUseTexture = false;

    // ----------------------------------------------------

    glGenFramebuffers(1, &m_FramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

    glGenTextures(1, &m_TextureID_ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ScreenWidth, m_ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID_ColorBuffer, 0);

    glGenRenderbuffers(1, &m_RenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, m_ScreenWidth, m_ScreenHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ----------------------------------------------------

    m_bIsRunning = true;

    return true;
}

void Application::Run()
{
    MSG msg;
    memset(&msg, 0, sizeof(msg));

    //LARGE_INTEGER frequency;
    //QueryPerformanceFrequency(&frequency);

    //LARGE_INTEGER previousTickCount;
    //QueryPerformanceCounter(&previousTickCount);

    std::chrono::_V2::system_clock::time_point previous;
    previous = std::chrono::high_resolution_clock::now();

    while (m_bIsRunning)
    {
        //LARGE_INTEGER currentTickCount;
        //QueryPerformanceCounter(&currentTickCount);

        //ULONGLONG elapsedTickCount = currentTickCount.QuadPart - previousTickCount.QuadPart;

        //ULONGLONG elapsedMicroseconds = (elapsedTickCount * 1'000'000);
        //elapsedMicroseconds /= frequency.QuadPart;

        //previousTickCount = currentTickCount;

        //float deltaTime = (elapsedMicroseconds * 0.001f) * 0.001f;

        std::chrono::_V2::system_clock::time_point current;
        current = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration<float>(current - previous).count();
        previous = current;

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                m_bIsRunning = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        const float cameraSpeed = 2.5f;

        const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec2 deltaMousePos = (m_CurrentMousePos - m_PrevMousePos) * 0.2f;
        m_PrevMousePos = m_CurrentMousePos;

        //if (m_bCameraMoving)
        //{
            if (m_bKeyStates[0x57]) //  W key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (m_Camera->GetForwardDir() * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x53]) // 	S key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (m_Camera->GetForwardDir() * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }

            const glm::vec3 rightDirection = glm::cross(m_Camera->GetForwardDir(), upDirection);

            if (m_bKeyStates[0x41]) //  A key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x44]) //  D key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (rightDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }

            if (m_bKeyStates[0x51]) //     Q key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (upDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x45])  //  E key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (upDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }

            /*
            //  sin(yaw) z     +            sin(pitch) y     +  
            //           |   / |                       |   / |
            //           | /   |                       | /   |
            //           ++ -  + x                     ++ -  + x/z
            //              cos(yaw)                        cos(pitch)
            */    

            if (m_bCameraMoving && !(deltaMousePos.x == 0.0f && deltaMousePos.y == 0.0f))
            {
                const float rotationSpeed = 0.55f;
                m_Yaw += deltaMousePos.x * rotationSpeed;
                m_Pitch += (-deltaMousePos.y) * rotationSpeed;

                glm::vec3 newForwardDirection;
                newForwardDirection.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                newForwardDirection.y = sinf(glm::radians(m_Pitch));
                newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                
                m_Camera->SetForwardDir(glm::normalize(newForwardDirection));
            }
        //}

        for (const auto& node : m_SceneNodes)
        {
            node->VUpdate(deltaTime);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
        glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Geometry phase
        StrongProgramPipelinePtr shaderProgram = m_AssetManager->GetShaderProgram("TexturedLit");
        shaderProgram->SetActive();
        shaderProgram->SetUniformMatrix4f("u_WorldViewProjection", m_Camera->WorldViewProjection());
        shaderProgram->SetUniform3f("u_ViewPos", m_Camera->GetPosition());

        for (uint32_t i = 0; i < m_LightNodes.size(); ++i)
        {
            const std::string index = std::to_string(i);
            const LightProperties& lp = m_LightNodes[i]->GetLightProperties();

            shaderProgram->SetUniform1i(("u_Lights[" + index + "].Type"), static_cast<int>(lp.Type));
            shaderProgram->SetUniform3f(("u_Lights[" + index + "].Position"), lp.Position);
            shaderProgram->SetUniform3f(("u_Lights[" + index + "].Direction"), lp.Direction);
            shaderProgram->SetUniform4f(("u_Lights[" + index + "].Ambient"), lp.Ambient);
            shaderProgram->SetUniform4f(("u_Lights[" + index + "].Diffuse"), lp.Diffuse);
            shaderProgram->SetUniform4f(("u_Lights[" + index + "].Specular"), lp.Specular);
            shaderProgram->SetUniform1f(("u_Lights[" + index + "].Falloff"), lp.Falloff);
            shaderProgram->SetUniform1f(("u_Lights[" + index + "].ConstantAttenuation"), lp.ConstantAttenuation);
            shaderProgram->SetUniform1f(("u_Lights[" + index + "].LinearAttenuation"), lp.LinearAttenuation);
            shaderProgram->SetUniform1f(("u_Lights[" + index + "].QuadraticAttenuation"), lp.QuadraticAttenuation);
        }

        for (const auto& node : m_SceneNodes) 
        {
            // Render scene nodes if not transparent, else add to the alpha scene node "queue".
            const float alpha = node->GetMaterial().Diffuse.a;

            if (alpha == 1.0f)
            {
                node->VRender();
            }
            else if (alpha == 0.0f)
            {
                AlphaSceneNode* pAlphaSceneNode = new AlphaSceneNode();
                pAlphaSceneNode->Node = node;

                m_AlphaSceneNodes.push_back(pAlphaSceneNode);
            }
        }

        // Render Skybox/Cubemap.
        glDepthFunc(GL_LEQUAL);
        StrongProgramPipelinePtr skyShaderProgram = m_AssetManager->GetShaderProgram("Sky");
        skyShaderProgram->SetActive();
        skyShaderProgram->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(m_Camera->GetView())));
        skyShaderProgram->SetUniformMatrix4f("u_WorldProjection", m_Camera->GetProjection());
        //m_Texture_Sky->BindUnit(0);
        m_Texture_Sky->SetActiveUnit(0);
        skyShaderProgram->SetUniform1i("u_Texture", 0);
        m_Mesh_Cube->m_VertexArray->SetActive();
        glDrawArrays(GL_TRIANGLES, m_Mesh_Cube->VertexBufferOffset, m_Mesh_Cube->VertexCount);
        glDepthFunc(GL_LESS);
        m_Texture_Sky->SetActive(GL_FALSE);

        // Render Alpha scene nodes.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
        for (uint32_t i = 0; i < m_AlphaSceneNodes.size(); i++)
        {
            for (uint32_t j = (i + 1); j < m_AlphaSceneNodes.size(); j++)
            {
                auto Vec3Length = [](const glm::vec3& p) -> float
                {
                    return (p.x * p.x + p.y * p.y + p.z * p.z);
                };

                if (Vec3Length(m_Camera->GetPosition() - m_AlphaSceneNodes[i]->Node->GetPosition()) 
                > Vec3Length(m_Camera->GetPosition() - m_AlphaSceneNodes[j]->Node->GetPosition()))
                {
                    AlphaSceneNode* pTempAlphaSceneNode = new AlphaSceneNode();
                    pTempAlphaSceneNode->Node = m_AlphaSceneNodes[i]->Node;

                    m_AlphaSceneNodes[i] = m_AlphaSceneNodes[j];
                    m_AlphaSceneNodes[j] = pTempAlphaSceneNode;
                }
            }
        }

        for (int i = (m_AlphaSceneNodes.size() - 1); i >= 0; i--)
        {
            m_AlphaSceneNodes[i]->Node->VRender();
            delete m_AlphaSceneNodes[i];
        }

        m_AlphaSceneNodes.clear();

        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        StrongProgramPipelinePtr framebufferTestShaderProgram = m_AssetManager->GetShaderProgram("FramebufferTest");
        framebufferTestShaderProgram->SetActive();
        m_Mesh_Rectangle->m_VertexArray->SetActive();
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
        glActiveTexture(GL_TEXTURE0);
        framebufferTestShaderProgram->SetUniform1i("u_Texture", 0);
        glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);

        wglSwapIntervalEXT(1);
        wglSwapLayerBuffers(m_hDeviceContext, WGL_SWAP_MAIN_PLANE);
    }
}

LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }

            m_bKeyStates[wParam] = true;

            //if (wParam == VK_F1)
            //{
            //    ShowCursor(FALSE);
            //    
            //}
            //else
            //if (wParam == VK_F2)
            //{
            //    ShowCursor(TRUE);
            //}

            //printf("Key Down: %llu \n", wParam);

            return 0;
        };

        case WM_KEYUP:
        {
            m_bKeyStates[wParam] = false;
            return 0;
        };

        case WM_MOUSEMOVE:
        {
            POINT pos;

            if (GetCursorPos(&pos))
            {
                ScreenToClient(m_hWindow, &pos);
                m_CurrentMousePos = glm::vec2(pos.x, pos.y);
            }

            //printf("Current Mouse Pos: x{%g} y{%g} \n", m_CurrentMousePos.x, m_CurrentMousePos.y);

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            m_bCameraMoving = true;
            return 0;
        }

        case WM_RBUTTONUP:
        {
            m_bCameraMoving = false;
            return 0;
        }

        case WM_CLOSE:
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(m_hWindow, uMsg, wParam, lParam);
}

void Application::InitWindow()
{
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = m_hInstance;
    wcex.lpszClassName = ClassName;

    ATOM wcexClassID = RegisterClassEx(&wcex);
    assert(wcexClassID);

    RECT wr = {0, 0, m_ScreenWidth, m_ScreenHeight};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    m_ScreenWidth = (wr.right - wr.left);
    m_ScreenHeight = (wr.bottom - wr.top);

    m_hWindow = CreateWindowEx(
        0,
        ClassName,
        TEXT("Render Engine | Testbed"),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, 
        (wr.right - wr.left), 
        (wr.bottom - wr.top),
    
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    assert(m_hWindow);
}

void Application::InitOpenGL()
{
    // Proper OpenGL context creation:
    //      https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
    //      ˘˘˘

    // Temp context:

    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = DefWindowProc;
    wcex.hInstance = m_hInstance;
    wcex.lpszClassName = TEXT("OpenGLContextCreation");

    ATOM wcexClassID = RegisterClassEx(&wcex);
    assert(wcexClassID);

    HWND hWindow = CreateWindowEx(
        0,
        TEXT("OpenGLContextCreation"),
        TEXT(""),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    
        nullptr,
        nullptr,
        m_hInstance,
        nullptr
    );

    HDC hDeviceContext = GetDC(hWindow);
    assert(hDeviceContext);

    PIXELFORMATDESCRIPTOR temp_pixelformatDescriptor;
    memset(&temp_pixelformatDescriptor, 0, sizeof(temp_pixelformatDescriptor));

    temp_pixelformatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    temp_pixelformatDescriptor.nVersion = 1;
    temp_pixelformatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    temp_pixelformatDescriptor.iPixelType = PFD_TYPE_RGBA;
    temp_pixelformatDescriptor.cColorBits = 32;
    temp_pixelformatDescriptor.cAlphaBits = 8;
    temp_pixelformatDescriptor.cDepthBits = 24;
    temp_pixelformatDescriptor.cStencilBits = 8;
    temp_pixelformatDescriptor.iLayerType = PFD_MAIN_PLANE;

    int temp_pixelformat = ChoosePixelFormat(hDeviceContext, &temp_pixelformatDescriptor);
    SetPixelFormat(hDeviceContext, temp_pixelformat, &temp_pixelformatDescriptor);

    HGLRC temp_hGLRenderContext = wglCreateContext(hDeviceContext);
    assert(temp_hGLRenderContext);

    WINBOOL bResult = wglMakeCurrent(hDeviceContext, temp_hGLRenderContext);
    assert(bResult);

    bResult = gladLoadWGL(hDeviceContext);
    assert(bResult);

    bResult = gladLoadGL();
    assert(bResult);

    wglDeleteContext(temp_hGLRenderContext);
    ReleaseDC(hWindow, hDeviceContext);

    DestroyWindow(hWindow);

    // Final context:

    m_hDeviceContext = GetDC(m_hWindow);
    assert(m_hDeviceContext);

    const int choosePixelformatAttribs[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End.
    };

    int pixelformat;
    UINT pixelformatCount;
    bResult = wglChoosePixelFormatARB(m_hDeviceContext, choosePixelformatAttribs, nullptr, 1, &pixelformat, &pixelformatCount);
    assert(bResult);

    PIXELFORMATDESCRIPTOR pixelformatDescriptor;
    DescribePixelFormat(m_hDeviceContext, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pixelformatDescriptor);
    SetPixelFormat(m_hDeviceContext, pixelformat, &pixelformatDescriptor);

    int createContextAttribs[] =
    {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
    };

    m_hGLRenderContext = wglCreateContextAttribsARB(m_hDeviceContext, nullptr, createContextAttribs);
    assert(m_hGLRenderContext);

    bResult = wglMakeCurrent(m_hDeviceContext, m_hGLRenderContext);
    assert(bResult);

    printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
}