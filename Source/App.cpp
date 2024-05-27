#include "App.h"

App* g_pApp = NULL;

std::shared_ptr<Texture_t> g_UvGridTexture = nullptr;

std::shared_ptr<Mesh_t> g_MonkeyMesh = nullptr;

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
    m_bCameraMoving = false;
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

    g_UvGridTexture.reset(new Texture_t());
    g_UvGridTexture->LoadFromFile("Assets/Textures/UvGrid.png");

    g_MonkeyMesh.reset(new Mesh_t());
    g_MonkeyMesh->LoadFromFile("Assets/Models/Monkey.obj");

    CreateScene();

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
            {
                m_bRunning = false;
            }

            if (event.type == SDL_KEYDOWN && event.key.repeat == SDL_FALSE)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_bRunning = false;
                }
                else
                {
                    m_bKeyStates[event.key.keysym.scancode] = true;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                m_bKeyStates[event.key.keysym.scancode] = false;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                m_CurrentMousePos.x = event.motion.xrel;
                m_CurrentMousePos.y = event.motion.yrel;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_bCameraMoving = true;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_bCameraMoving = false;
                }
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

    Material_t Material;
    Material.bUseTexture = true;

    std::shared_ptr<MeshSceneNode> suzanneTheMonkey(new MeshSceneNode("Assets/Models/Monkey.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    suzanneTheMonkey->Create();
    suzanneTheMonkey->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    suzanneTheMonkey->SetMaterial(Material);
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

    /** Directional Light */

    LightProperties_t DirectionalLightProperties;
    DirectionalLightProperties.Type = LightType_t::Directional;
    DirectionalLightProperties.Direction = glm::vec3(1.2f, -1.0f, 1.3f);

    std::shared_ptr<LightSceneNode_t> DirectionalLight(new LightSceneNode_t(DirectionalLightProperties));

    m_SceneNodes.push_back(DirectionalLight);
    m_LightNodes.push_back(DirectionalLight);

    /** Point Light */

    LightProperties_t PointLightProperties;
    PointLightProperties.Type = LightType_t::Point;
    PointLightProperties.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    PointLightProperties.ConstantAttenuation = 1.0f;
    PointLightProperties.LinearAttenuation = 0.09f;
    PointLightProperties.QuadraticAttenuation = 0.032f;

    std::shared_ptr<LightSceneNode_t> PointLight(new LightSceneNode_t(PointLightProperties));
    PointLight->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));

    m_SceneNodes.push_back(PointLight);
    m_LightNodes.push_back(PointLight);

    /** Spot Light */

    LightProperties_t SpotLightProperties;
    SpotLightProperties.Type = LightType_t::Spot;
    SpotLightProperties.Position = glm::vec3(3.4f, 1.0f, 3.0f);
    SpotLightProperties.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    SpotLightProperties.ConstantAttenuation = 1.0f;
    SpotLightProperties.LinearAttenuation = 0.09f;
    SpotLightProperties.QuadraticAttenuation = 0.032f;
    SpotLightProperties.Falloff = cosf(glm::radians(30.0f));
    SpotLightProperties.Phi = cosf(glm::radians(36.0f));

    std::shared_ptr<LightSceneNode_t> SpotLight(new LightSceneNode_t(SpotLightProperties));
    SpotLight->SetPosition(glm::vec3(0.4f, 1.0f, -2.0f));

    m_SceneNodes.push_back(SpotLight);
    m_LightNodes.push_back(SpotLight);

    /** Sky */

    SkySceneNode.reset(new SkySceneNode_t());
}

void App::UpdateScene(const float deltaTime)
{
    for (const std::shared_ptr<SceneNode_t>& node : m_SceneNodes)
        node->Update(deltaTime);
}

void App::RenderScene()
{
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
        const LightProperties_t& lp = m_LightNodes[i]->GetProperties();

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

    SkySceneNode->Render();

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
}