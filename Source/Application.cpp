#include "Application.h"

Application* g_pApp = NULL;
StrongProgramPipelinePtr g_TexturedLitShader = NULL;
StrongProgramPipelinePtr g_SkyShader = NULL;
StrongProgramPipelinePtr g_BillboardShader = NULL;

////////////////////////////////////////////////////
//  Application Implementation
////////////////////////////////////////////////////

Application::Application()
{
    g_pApp = this;

    m_bRunning = false;
    
    m_pWindow = nullptr;
    m_Context = nullptr;

    m_pScene = nullptr;
}

Application::~Application()
{
    if (m_pScene)
        delete m_pScene;

    if (m_Context)
        SDL_GL_DeleteContext(m_Context);

    if (m_pWindow)
        SDL_DestroyWindow(m_pWindow);

    SDL_Quit();

    if (g_pApp)
        g_pApp = nullptr;
}

bool Application::Initialize()
{
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

    m_Context = SDL_GL_CreateContext(m_pWindow);
    assert(m_Context != NULL);

    int bGladLoadGLResult = gladLoadGL();
    assert(bGladLoadGLResult != GL_FALSE);

    memset(m_bKeyStates, 0, sizeof(m_bKeyStates));
    m_CurrentMousePos = glm::vec2(0.0f, 0.0f);
    m_PrevMousePos = m_CurrentMousePos;

    g_TexturedLitShader.reset(new ProgramPipeline());
    g_TexturedLitShader->LoadFromFile("Assets/Shaders/TexturedLit.vert", "Assets/Shaders/TexturedLit.frag");

    g_SkyShader.reset(new ProgramPipeline());
    g_SkyShader->LoadFromFile("Assets/Shaders/Sky.vert", "Assets/Shaders/Sky.frag");

    g_BillboardShader.reset(new ProgramPipeline());
    g_BillboardShader->LoadFromFile("Assets/Shaders/Billboard.vert", "Assets/Shaders/Billboard.frag");

    m_pScene = new Scene();

    m_bCameraMoving = false;
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    m_bRunning = true;
    
    return true;
}

void Application::RunLoop()
{
    uint64_t prevTime = (uint64_t)SDL_GetPerformanceCounter();

    while (m_bRunning)
    {
        ProcessEvents();

        uint64_t currentTime = (uint64_t)SDL_GetPerformanceCounter();
        float deltaTime = ((float)(currentTime - prevTime) / (float)SDL_GetPerformanceFrequency());
        prevTime = currentTime;

        /**  */

        const float cameraSpeed = 25.0f;
        const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec2 deltaMousePos = (m_CurrentMousePos - m_PrevMousePos) * 0.2f;
        m_PrevMousePos = m_CurrentMousePos;

        if (m_bCameraMoving)
        {
            std::shared_ptr<CameraNode>& camera = m_pScene->GetCamera();
            std::shared_ptr<SceneNode>& cameraTargetNode = camera->m_TargetNode;

            if (!m_bDebugCameraEnabled)
            {
                if (m_bKeyStates[SDL_SCANCODE_W])
                {
                    glm::vec3 newPos = cameraTargetNode->GetPosition() + (camera->GetForwardDir() * cameraSpeed * deltaTime);
                    cameraTargetNode->SetPosition(newPos);
                }
                else
                if (m_bKeyStates[SDL_SCANCODE_S])
                {
                    glm::vec3 newPos = cameraTargetNode->GetPosition() - (camera->GetForwardDir() * cameraSpeed * deltaTime);
                    cameraTargetNode->SetPosition(newPos);
                }

                const glm::vec3 rightDirection = glm::cross(camera->GetForwardDir(), upDirection);

                if (m_bKeyStates[SDL_SCANCODE_A])
                {
                    glm::vec3 newPos = cameraTargetNode->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                    cameraTargetNode->SetPosition(newPos);
                }
                else
                if (m_bKeyStates[SDL_SCANCODE_D])
                {
                    glm::vec3 newPos = cameraTargetNode->GetPosition() + (rightDirection * cameraSpeed * deltaTime);
                    cameraTargetNode->SetPosition(newPos);
                }

                if (!(deltaMousePos.x == 0.0f && deltaMousePos.y == 0.0f))
                {
                    const float rotationSpeed = 0.3f;
                    m_Yaw += deltaMousePos.x * rotationSpeed;
                    m_Pitch += (-deltaMousePos.y) * rotationSpeed;

                    glm::vec3 newForwardDirection;
                    newForwardDirection.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                    newForwardDirection.y = sinf(glm::radians(m_Pitch));
                    newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                    
                    camera->SetForwardDir(glm::normalize(newForwardDirection));
                }

                camera->m_TargetPos = camera->m_TargetNode->GetPosition();
                camera->SetPosition(camera->m_TargetNode->GetPosition() - camera->GetForwardDir() * 15.0f);
                camera->WorldViewProjection();
            }
            else
            {
                if (m_bKeyStates[SDL_SCANCODE_W])
                {
                    glm::vec3 newPos = camera->GetPosition() + (camera->GetForwardDir() * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }
                else
                if (m_bKeyStates[SDL_SCANCODE_S])
                {
                    glm::vec3 newPos = camera->GetPosition() - (camera->GetForwardDir() * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }

                const glm::vec3 rightDirection = glm::cross(camera->GetForwardDir(), upDirection);

                if (m_bKeyStates[SDL_SCANCODE_A])
                {
                    glm::vec3 newPos = camera->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }
                else
                if (m_bKeyStates[SDL_SCANCODE_D])
                {
                    glm::vec3 newPos = camera->GetPosition() + (rightDirection * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }

                if (m_bKeyStates[SDL_SCANCODE_LSHIFT])
                {
                    glm::vec3 newPos = camera->GetPosition() + (upDirection * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }
                else
                if (m_bKeyStates[SDL_SCANCODE_LCTRL])
                {
                    glm::vec3 newPos = camera->GetPosition() - (upDirection * cameraSpeed * deltaTime);
                    camera->SetPosition(newPos);
                }

                if (!(deltaMousePos.x == 0.0f && deltaMousePos.y == 0.0f))
                {
                    const float rotationSpeed = 0.3f;
                    m_Yaw += deltaMousePos.x * rotationSpeed;
                    m_Pitch += (-deltaMousePos.y) * rotationSpeed;

                    glm::vec3 newForwardDirection;
                    newForwardDirection.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                    newForwardDirection.y = sinf(glm::radians(m_Pitch));
                    newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                    
                    camera->SetForwardDir(glm::normalize(newForwardDirection));
                }

                camera->m_TargetPos = camera->GetPosition() + camera->GetForwardDir();
                camera->WorldViewProjection();
            }
        }

        m_pScene->Update(deltaTime);

        m_pScene->Render();

        SDL_GL_SetSwapInterval(0);
        SDL_GL_SwapWindow(m_pWindow);
    }
}

void Application::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_bRunning = false;
                break;

            case SDL_KEYDOWN:
            {
                if (event.key.repeat != SDL_FALSE)
                    break;

                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    m_bRunning = false;
                else if (event.key.keysym.scancode == SDL_SCANCODE_Q)
                {
                    m_bDebugCameraEnabled = !m_bDebugCameraEnabled;

                    if (m_bDebugCameraEnabled)
                        printf("Debug camera enabled.\n");
                    else
                        printf("Debug camera disabled.\n");
                }
                else
                    m_bKeyStates[event.key.keysym.scancode] = true;

                break;
            }

            case SDL_KEYUP:
                m_bKeyStates[event.key.keysym.scancode] = false;
                break;

            case SDL_MOUSEMOTION:
            {
                m_CurrentMousePos.x = event.motion.x;
                m_CurrentMousePos.y = event.motion.y;
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    m_bCameraMoving = true;

                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    m_bCameraMoving = false;

                break;
            }

            default:
                break;
        }
    }
}