#include "Application.h"

Application* g_pApp = nullptr;

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

Application::Application()
{
    g_pApp = this;

    m_bRunning = false;

    m_pWindow = nullptr;
    m_pGLContext = nullptr;

    m_currentTime = 0;
    m_lastTime = 0;
    
    m_deltaTime = 0;

    m_pScene = nullptr;
}

Application::~Application()
{
    if (m_pScene)
    {
        delete m_pScene;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_pGLContext)
    {
        SDL_GL_DeleteContext(m_pGLContext);
    }

    if (m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
    }

    SDL_Quit();

    if (g_pApp)
    {
        g_pApp = nullptr;
    }
}

bool Application::Init()
{
    // Setup the window and OpenGL.
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return false;
    }

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
    
    if (!m_pWindow)
    {
        return false;
    }

    m_pGLContext = SDL_GL_CreateContext(m_pWindow);
    
    if (!m_pGLContext)
    {
        return false;
    }

    if (!gladLoadGL())
    {
        return false;
    }

    // Setup Input "system".
    memset(m_bKeys, 0, sizeof(m_bKeys));

    m_currentMousePos = glm::vec2(0.0f, 0.0f);
    m_lastMousePos = m_currentMousePos;

    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends.
    ImGui_ImplSDL2_InitForOpenGL(m_pWindow, m_pGLContext);
    ImGui_ImplOpenGL3_Init();

    // Setup the scene.
    m_pScene = new Scene();
    m_pScene->Init();

    m_bCameraMoving = false;
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    // End.
    m_bRunning = true;
    
    return true;
}

void Application::RunLoop()
{
    m_lastTime = (uint64_t)SDL_GetPerformanceCounter();

    while (m_bRunning)
    {
        ProcessMessages();

        m_currentTime = (uint64_t)SDL_GetPerformanceCounter();
        m_deltaTime = ((float)(m_currentTime - m_lastTime) / (float)SDL_GetPerformanceFrequency());
        m_lastTime = m_currentTime;

        Update(m_deltaTime);
        Render();
    }
}

void Application::ProcessMessages()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        if (OnImGUIMsgProc(event))
            continue;

        OnMsgProc(event);
    }
}

bool Application::OnMsgProc(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT:
            m_bRunning = false;
            return true;

        case SDL_KEYDOWN:
        {
            if (event.key.repeat != SDL_FALSE)
                return false;

            return OnKeyDown(event.key.keysym.scancode);
        }

        case SDL_KEYUP:
            return OnKeyUp(event.key.keysym.scancode);

        case SDL_MOUSEMOTION:
            return OnMouseMove(glm::ivec2(event.motion.x, event.motion.y));

        case SDL_MOUSEBUTTONDOWN:
            return OnMouseButtonDown(event.button.button);        

        case SDL_MOUSEBUTTONUP:
            return OnMouseButtonUp(event.button.button);        
    }

    return false;
}

bool Application::OnImGUIMsgProc(const SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard || !io.WantCaptureMouse)
        return false;

    switch (event.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            return true;
    }

    return false;
}

bool Application::OnKeyDown(int key)
{
    if (!(0 <= key && key <= MAX_KEYS))
        return false;

    switch (key)
    {
        case SDL_SCANCODE_ESCAPE:
        {
            SDL_Event event;
            event.type = SDL_QUIT;
            SDL_PushEvent(&event);

            return true;
        }

        case SDL_SCANCODE_Q:
        {
            m_bDebugCameraEnabled = !m_bDebugCameraEnabled;
            return true;
        }
    }

    m_bKeys[key] = true;

    return true;
}

bool Application::OnKeyUp(int key)
{
    if (!(0 <= key && key <= MAX_KEYS))
        return false;

    m_bKeys[key] = false;

    return true;
}

bool Application::OnMouseMove(const glm::ivec2& pos)
{
    m_currentMousePos = pos;
    return true;
}

bool Application::OnMouseButtonDown(int button)
{
    if (button == SDL_BUTTON_RIGHT)
    {
        m_bCameraMoving = true;
        return true;
    }

    return false;
}

bool Application::OnMouseButtonUp(int button)
{
    if (button == SDL_BUTTON_RIGHT)
    {
        m_bCameraMoving = false;
        return true;
    }

    return false;
}

void Application::Update(const float deltaTime)
{
    UpdateMovementController(deltaTime);
    m_pScene->Update(deltaTime);

    m_performanceInfoControl.Update(deltaTime);
}

void Application::UpdateMovementController(const float deltaTime)
{
    const float cameraSpeed = 25.0f;
    const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec2 deltaMousePos = (m_currentMousePos - m_lastMousePos) * 0.2f;
    m_lastMousePos = m_currentMousePos;

    if (m_bCameraMoving)
    {
        std::shared_ptr<CameraNode> camera = m_pScene->GetCamera();
        std::shared_ptr<SceneNode>& cameraTargetNode = camera->m_TargetNode;

        if (!m_bDebugCameraEnabled)
        {
            if (m_bKeys[SDL_SCANCODE_W])
            {
                glm::vec3 newPos = cameraTargetNode->GetPosition() + (camera->GetForwardDir() * cameraSpeed * deltaTime);
                cameraTargetNode->SetPosition(newPos);
            }
            else
            if (m_bKeys[SDL_SCANCODE_S])
            {
                glm::vec3 newPos = cameraTargetNode->GetPosition() - (camera->GetForwardDir() * cameraSpeed * deltaTime);
                cameraTargetNode->SetPosition(newPos);
            }

            const glm::vec3 rightDirection = glm::cross(camera->GetForwardDir(), upDirection);

            if (m_bKeys[SDL_SCANCODE_A])
            {
                glm::vec3 newPos = cameraTargetNode->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                cameraTargetNode->SetPosition(newPos);
            }
            else
            if (m_bKeys[SDL_SCANCODE_D])
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
            if (m_bKeys[SDL_SCANCODE_W])
            {
                glm::vec3 newPos = camera->GetPosition() + (camera->GetForwardDir() * cameraSpeed * deltaTime);
                camera->SetPosition(newPos);
            }
            else
            if (m_bKeys[SDL_SCANCODE_S])
            {
                glm::vec3 newPos = camera->GetPosition() - (camera->GetForwardDir() * cameraSpeed * deltaTime);
                camera->SetPosition(newPos);
            }

            const glm::vec3 rightDirection = glm::cross(camera->GetForwardDir(), upDirection);

            if (m_bKeys[SDL_SCANCODE_A])
            {
                glm::vec3 newPos = camera->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                camera->SetPosition(newPos);
            }
            else
            if (m_bKeys[SDL_SCANCODE_D])
            {
                glm::vec3 newPos = camera->GetPosition() + (rightDirection * cameraSpeed * deltaTime);
                camera->SetPosition(newPos);
            }

            if (m_bKeys[SDL_SCANCODE_LSHIFT])
            {
                glm::vec3 newPos = camera->GetPosition() + (upDirection * cameraSpeed * deltaTime);
                camera->SetPosition(newPos);
            }
            else
            if (m_bKeys[SDL_SCANCODE_LCTRL])
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
}

void Application::Render()
{
    // Start the frame.
    m_pScene->Render();
    ImGUIRender();

    // End the frame.
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SwapWindow(m_pWindow);
}

static std::shared_ptr<SceneNode> selected = nullptr;

static void ImGUIRenderSceneNode(std::shared_ptr<SceneNode> node)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

    if (selected == node)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (node->GetChildren().empty()) 
        flags |= ImGuiTreeNodeFlags_Leaf;

    bool bTreeNodeOpen = ImGui::TreeNodeEx(node->GetName().c_str(), flags);

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        selected = node;
    }

    if (bTreeNodeOpen)
    {
        SceneNodeVector::const_iterator it = node->GetChildren().begin();
        
        while (it != node->GetChildren().end())
        {
            ImGUIRenderSceneNode((*it));
            ++it;
        }

        ImGui::TreePop();
    }
}

void Application::ImGUIRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Show demo.
    ImGui::ShowDemoWindow();

    // Show Node Control.
    ImGui::Begin("Scene Graph");
    
    std::shared_ptr<SceneNode> root = m_pScene->GetRoot();
    ImGUIRenderSceneNode(root);

    ImGui::End();

    m_performanceInfoControl.Render();

    // End the Dear ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}