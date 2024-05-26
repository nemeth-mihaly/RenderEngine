#include "App.h"

App* g_pApp = NULL;

////////////////////////////////////////////////////
//  App Implementation
////////////////////////////////////////////////////

App::App()
{
    g_pApp = this;

    m_bRunning = false;
    
    m_pWindow = NULL;
    m_pContext = NULL;

    m_pResManager = NULL;
}

App::~App()
{
    if (m_pResManager)
        delete m_pResManager;

    if (m_pContext)
        SDL_GL_DeleteContext(m_pContext);

    if (m_pWindow)
        SDL_DestroyWindow(m_pWindow);

    SDL_Quit();

    if (g_pApp)
        g_pApp = NULL;
}

bool App::Init()
{
    m_pResManager = new ResourceManager();
    assert(m_pResManager != NULL);

    int bSDLInitResult = SDL_Init(SDL_INIT_EVERYTHING);
    assert(bSDLInitResult >= 0);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    m_pWindow = SDL_CreateWindow("Render Engine | Testbed", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
    assert(m_pWindow != NULL);

    m_pContext = SDL_GL_CreateContext(m_pWindow);
    assert(m_pContext != NULL);

    int bGladLoadGLResult = gladLoadGL();
    assert(bGladLoadGLResult != GL_FALSE);

    memset(m_bKeyStates, 0, sizeof(m_bKeyStates));
    m_CurrentMousePos = glm::vec2(0.0f, 0.0f);
    m_PrevMousePos = m_CurrentMousePos;
    m_bCameraMoving = true;
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    const std::vector<std::string> programPipelineResources =
    {
        "Assets/Shaders/TexturedLit.progpipeline",
        "Assets/Shaders/Sky.progpipeline",
        "Assets/Shaders/Framebuffer_test.progpipeline"
    };

    for (const auto& programPipelineResource : programPipelineResources)
        g_pApp->GetResourceManager().LoadProgramPipeline(programPipelineResource);

    const std::vector<std::string> wavefrontModelResources =
    {
        "Assets\\Models\\Rectangle.obj",
        "Assets\\Models\\Cube.obj",
        "Assets/Models/Monkey.obj",
        "Assets\\Models\\Grass.obj"
    };

    for (const auto& wavefrontModelResource : wavefrontModelResources)
        g_pApp->GetResourceManager().LoadWavefrontMesh(wavefrontModelResource);

    const std::vector<std::string> textureResources =
    {
        "Assets/Textures/UvGrid.png",
        "Assets\\Textures\\Stonebricks.png",
        "Assets\\Textures\\Grass.png",
        "Assets\\Textures\\SphereGlow.png",
        "Assets\\Textures\\AlphaWindow.png"
    };

    for (const auto& textureResource : textureResources)
        g_pApp->GetResourceManager().LoadTexture(textureResource);

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

    CreateScene();

    glGenFramebuffers(1, &m_FramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

    glGenTextures(1, &m_TextureID_ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID_ColorBuffer, 0);

    glGenRenderbuffers(1, &m_RenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, 1280, 720);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_bRunning = true;
    
    return true;
}

void App::RunLoop()
{
    uint64_t uPreviousTime = (uint64_t)SDL_GetPerformanceCounter();

    while (m_bRunning)
    {
        /** Process Events */

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                m_bRunning = false;

            if (event.type == SDL_KEYDOWN && event.key.repeat == SDL_FALSE)
                m_bKeyStates[event.key.keysym.scancode] = true;
            else if (event.type == SDL_KEYUP)
            {
                m_bKeyStates[event.key.keysym.scancode] = false;
            }
        }

        /** Calc Delta Time */

        uint64_t uCurrentTime = (uint64_t)SDL_GetPerformanceCounter();
        float fDeltaTime = ((float)(uCurrentTime - uPreviousTime) / (float)SDL_GetPerformanceFrequency());
        uPreviousTime = uCurrentTime;

        /**  */

        const float cameraSpeed = 2.5f;
        const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec2 deltaMousePos = (m_CurrentMousePos - m_PrevMousePos) * 0.2f;
        m_PrevMousePos = m_CurrentMousePos;

        if (m_bCameraMoving)
        {
            if (m_bKeyStates[SDL_SCANCODE_W])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() + (m_CameraSceneNode->GetForwardDir() * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[SDL_SCANCODE_S])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() - (m_CameraSceneNode->GetForwardDir() * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }

            const glm::vec3 rightDirection = glm::cross(m_CameraSceneNode->GetForwardDir(), upDirection);

            if (m_bKeyStates[SDL_SCANCODE_A])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() - (rightDirection * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[SDL_SCANCODE_D])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() + (rightDirection * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }

            if (m_bKeyStates[SDL_SCANCODE_LSHIFT])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() + (upDirection * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[SDL_SCANCODE_LCTRL])
            {
                glm::vec3 newPos = m_CameraSceneNode->GetPosition() - (upDirection * cameraSpeed * fDeltaTime);
                m_CameraSceneNode->SetPosition(newPos);
            }

            if (m_bCameraMoving && !(deltaMousePos.x == 0.0f && deltaMousePos.y == 0.0f))
            {
                const float rotationSpeed = 0.3f;
                m_Yaw += deltaMousePos.x * rotationSpeed;
                m_Pitch += (-deltaMousePos.y) * rotationSpeed;

                glm::vec3 newForwardDirection;
                newForwardDirection.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                newForwardDirection.y = sinf(glm::radians(m_Pitch));
                newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                
                m_CameraSceneNode->SetForwardDir(glm::normalize(newForwardDirection));
            }
        }

        UpdateScene(fDeltaTime);

        RenderScene();

        SDL_GL_SwapWindow(m_pWindow);
    }
}

void App::CreateScene()
{
    m_CameraSceneNode.reset(new CameraSceneNode());
    m_CameraSceneNode->Create();
    m_CameraSceneNode->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_SceneNodes.push_back(m_CameraSceneNode);

    std::shared_ptr<MeshSceneNode> suzanneTheMonkey(new MeshSceneNode("Assets/Models/Monkey.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    suzanneTheMonkey->Create();
    suzanneTheMonkey->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    suzanneTheMonkey->GetMaterial().bUseTexture = true;
    m_SceneNodes.push_back(suzanneTheMonkey);

    //auto floorNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //floorNode->Create();
    //floorNode->SetPosition(glm::vec3(0.0f, -0.55f, -0.0f));
    //floorNode->SetScale(glm::vec3(10.0f, 0.1f, 10.0f));
    //floorNode->GetMaterial().Diffuse = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
    //floorNode->GetMaterial().Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //floorNode->GetMaterial().bUseTexture = false;

    //auto node1 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node1->Create();
    //node1->SetPosition(glm::vec3(-1.5f, 0.0f, -2.5f));
    //node1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node1->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node1->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node1->GetMaterial().bUseTexture = true;

    //auto node2 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node2->Create();
    //node2->SetPosition(glm::vec3(2.0f, 0.0f, -2.5f));
    //node2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node2->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node2->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node2->GetMaterial().bUseTexture = true;

    //auto node3 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node3->Create();
    //node3->SetPosition(glm::vec3(1.0f, 0.0f, -3.5f));
    //node3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node3->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node3->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node3->GetMaterial().bUseTexture = true;

    //auto monkeyNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets/Models/Monkey.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //monkeyNode->Create();
    //monkeyNode->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    //monkeyNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //monkeyNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //monkeyNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //monkeyNode->GetMaterial().bUseTexture = true;

    //std::vector<glm::vec3> grassPositions =
    //{
    //    glm::vec3(-1.5f, 0.0f, -0.48f),
    //    glm::vec3( 1.5f, 0.0f, 0.51f),
    //    glm::vec3( 0.0f, 0.0f, 0.7f),
    //    glm::vec3(-0.3f, 0.0f, -2.3f),
    //    glm::vec3(0.5f, 0.0f, -0.6f)
    //};

    //for (uint32_t i = 0; i < grassPositions.size(); ++i)
    //{
    //    auto grassNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Grass.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets\\Textures\\Grass.png"));
    //    grassNode->Create();
    //    grassNode->SetPosition(grassPositions[i]);
    //    grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //    grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //    grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //    grassNode->GetMaterial().bUseTexture = true; 
    //}

    //std::vector<glm::vec3> glowPositions =
    //{
    //    glm::vec3(-1.5f, 0.3f, 0.48f),
    //    glm::vec3( 1.5f, 0.3f, 1.51f),
    //    glm::vec3( 0.0f, 0.3f, 1.7f),
    //    glm::vec3(-0.3f, 0.3f, 2.3f),
    //    glm::vec3(0.5f, 0.3f, 0.6f)
    //};

    //for (uint32_t i = 0; i < glowPositions.size(); ++i)
    //{
    //    std::string textureName = "Assets\\Textures\\SphereGlow.png";
//
    //    if (i % 2 == 0)
    //    {
    //        textureName = "Assets\\Textures\\AlphaWindow.png";
    //    }
//
    //    auto grassNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Rectangle.obj", "Assets/Shaders/TexturedLit.progpipeline", textureName));
    //    grassNode->Create();
    //    grassNode->SetPosition(glowPositions[i]);
    //    grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //    grassNode->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
    //    grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    //    grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //    grassNode->GetMaterial().bUseTexture = true; 
    //}

    using enum LightType;

    /** Directional Light */

    LightProperties dirLightProps;
    dirLightProps.Type = Directional;
    dirLightProps.Direction = glm::vec3(1.2f, -1.0f, 1.3f);

    std::shared_ptr<LightSceneNode> dirLightNode(new LightSceneNode(dirLightProps));

    m_SceneNodes.push_back(dirLightNode);
    m_LightNodes.push_back(dirLightNode);

    /** Point Light */

    LightProperties pointLightProps;
    pointLightProps.Type = Point;
    pointLightProps.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    pointLightProps.ConstantAttenuation = 1.0f;
    pointLightProps.LinearAttenuation = 0.09f;
    pointLightProps.QuadraticAttenuation = 0.032f;

    std::shared_ptr<LightSceneNode> pointLightNode(new LightSceneNode(pointLightProps));
    pointLightNode->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));

    m_SceneNodes.push_back(pointLightNode);
    m_LightNodes.push_back(pointLightNode);

    /** Spot Light */

    LightProperties spotLightProperties;
    spotLightProperties.Type = LightType::Spot;
    spotLightProperties.Position = glm::vec3(3.4f, 1.0f, 3.0f);
    spotLightProperties.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLightProperties.ConstantAttenuation = 1.0f;
    spotLightProperties.LinearAttenuation = 0.09f;
    spotLightProperties.QuadraticAttenuation = 0.032f;
    spotLightProperties.Falloff = cosf(glm::radians(30.0f));
    spotLightProperties.Phi = cosf(glm::radians(36.0f));

    std::shared_ptr<LightSceneNode> spotLightNode(new LightSceneNode(spotLightProperties));
    spotLightNode->SetPosition(glm::vec3(0.4f, 1.0f, -2.0f));

    m_SceneNodes.push_back(spotLightNode);
    m_LightNodes.push_back(spotLightNode);
}

void App::UpdateScene(const float deltaTime)
{
    for (const std::shared_ptr<SceneNode>& node : m_SceneNodes)
        node->Update(deltaTime);
}

void App::RenderScene()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

    /** Actor Render Pass */

    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    StrongProgramPipelinePtr shaderProgram = g_pApp->GetResourceManager().GetProgramPipeline("Assets/Shaders/TexturedLit.progpipeline");
    shaderProgram->SetActive();
    shaderProgram->SetUniformMatrix4f("u_WorldViewProjection", m_CameraSceneNode->WorldViewProjection());
    shaderProgram->SetUniform3f("u_ViewPos", m_CameraSceneNode->GetPosition());

    for (uint32_t i = 0; i < m_LightNodes.size(); ++i)
    {
        const std::string index = std::to_string(i);
        const LightProperties& lp = m_LightNodes[i]->GetLightProperties();

        shaderProgram->SetUniform1i(("u_Lights[" + index + "].Type"), static_cast<int>(lp.Type));
        shaderProgram->SetUniform3f(("u_Lights[" + index + "].Position"), lp.Position);
        shaderProgram->SetUniform3f(("u_Lights[" + index + "].Direction"), lp.Direction);
        shaderProgram->SetUniform4f(("u_Lights[" + index + "].Ambient"), m_LightNodes[i]->GetMaterial().Ambient);
        shaderProgram->SetUniform4f(("u_Lights[" + index + "].Diffuse"), m_LightNodes[i]->GetMaterial().Diffuse);
        shaderProgram->SetUniform4f(("u_Lights[" + index + "].Specular"), m_LightNodes[i]->GetMaterial().Specular);
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
            node->Render();
        }
        else if (alpha == 0.0f)
        {
            AlphaSceneNode* pAlphaSceneNode = new AlphaSceneNode();
            pAlphaSceneNode->Node = node;

            m_AlphaSceneNodes.push_back(pAlphaSceneNode);
        }
    }

    /** Sky Render Pass */

    glDepthFunc(GL_LEQUAL);
    StrongProgramPipelinePtr skyShaderProgram = g_pApp->GetResourceManager().GetProgramPipeline("Assets/Shaders/Sky.progpipeline");
    skyShaderProgram->SetActive();
    skyShaderProgram->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(m_CameraSceneNode->GetView())));
    skyShaderProgram->SetUniformMatrix4f("u_WorldProjection", m_CameraSceneNode->GetProjection());
    //m_Texture_Sky->BindUnit(0);
    m_Texture_Sky->SetActiveUnit(0);
    skyShaderProgram->SetUniform1i("u_Texture", 0);
    m_Mesh_Cube->m_VertexArray->SetActive();
    glDrawArrays(GL_TRIANGLES, m_Mesh_Cube->VertexBufferOffset, m_Mesh_Cube->VertexCount);
    glDepthFunc(GL_LESS);
    m_Texture_Sky->SetActive(GL_FALSE);

    /** Alpha Render Pass */

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

            if (Vec3Length(m_CameraSceneNode->GetPosition() - m_AlphaSceneNodes[i]->Node->GetPosition()) 
            > Vec3Length(m_CameraSceneNode->GetPosition() - m_AlphaSceneNodes[j]->Node->GetPosition()))
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
        m_AlphaSceneNodes[i]->Node->Render();
        delete m_AlphaSceneNodes[i];
    }

    m_AlphaSceneNodes.clear();

    glDisable(GL_BLEND);

    /** Postproc */

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    StrongProgramPipelinePtr framebufferTestShaderProgram = g_pApp->GetResourceManager().GetProgramPipeline("Assets/Shaders/Framebuffer_test.progpipeline");
    framebufferTestShaderProgram->SetActive();
    m_Mesh_Rectangle->m_VertexArray->SetActive();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
    glActiveTexture(GL_TEXTURE0);
    framebufferTestShaderProgram->SetUniform1i("u_Texture", 0);
    glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);
}