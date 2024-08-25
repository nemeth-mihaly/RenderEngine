#include "Application.h"

#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

Application* g_pApp = nullptr;

static void GlfwWindowCloseCallback([[maybe_unused]] GLFWwindow* pWindow) { g_pApp->GetEventManager().TriggerEvent(std::make_shared<AppQuitEvent>()); }

static void GlfwCursorPosCallback([[maybe_unused]] GLFWwindow* pWindow, double x, double y) { g_pApp->GetEventManager().TriggerEvent(std::make_shared<MouseMoveEvent>(x, y)); }

static void GlfwMouseButtonCallback([[maybe_unused]] GLFWwindow* pWindow, int button, int action, [[maybe_unused]] int mods)
{
    if (action == GLFW_PRESS)
    {
        g_pApp->GetEventManager().TriggerEvent(std::make_shared<MouseButtonDownEvent>(button));
    }
    else if (action == GLFW_RELEASE)
    {
        g_pApp->GetEventManager().TriggerEvent(std::make_shared<MouseButtonUpEvent>(button));
    } 
}

static void GlfwKeyCallback([[maybe_unused]] GLFWwindow* pWindow, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
    if (action == GLFW_PRESS)
    {
        g_pApp->GetEventManager().TriggerEvent(std::make_shared<KeyDownEvent>(key));
    }
    else if (action == GLFW_RELEASE)
    {
        g_pApp->GetEventManager().TriggerEvent(std::make_shared<KeyUpEvent>(key));
    }
}

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

Application::Application()
{
    g_pApp = this;

    m_bRunning = false;

    m_pWindow = nullptr;

    m_MousePos = glm::vec2(0.0f, 0.0f);
    m_LastMousePos = m_MousePos;
    m_bRightMouseButtonDown = false;
    memset(m_bKeys, 0x00, sizeof(m_bKeys));

    m_LastActorId = 0;
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_pWindow) { glfwDestroyWindow(m_pWindow); }
    glfwTerminate();
}

void Application::OnEvent(std::shared_ptr<IEvent> event)
{
    switch (event->GetType())
    {
        case AppQuitEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<AppQuitEvent>(event);
            m_bRunning = false;

            break;
        }

        case MouseMoveEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<MouseMoveEvent>(event);
            m_MousePos.x = evt->GetX();
            m_MousePos.y = evt->GetY();

            break;
        }

        case MouseButtonDownEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<MouseButtonDownEvent>(event);

            if (evt->GetButtonId() == GLFW_MOUSE_BUTTON_RIGHT)
            {
                m_bRightMouseButtonDown = true;
            }

            break;
        }

        case MouseButtonUpEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<MouseButtonUpEvent>(event);

            if (evt->GetButtonId() == GLFW_MOUSE_BUTTON_RIGHT)
            {
                m_bRightMouseButtonDown = false;
            }

            break;
        }    

        case KeyDownEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<KeyDownEvent>(event);

            int keyId = evt->GetKeyId();
            if (keyId == GLFW_KEY_ESCAPE)
            {
                m_bRunning = false;
            }
            
            m_bKeys[keyId] = true;

            break;            
        }

        case KeyUpEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<KeyUpEvent>(event);
            m_bKeys[evt->GetKeyId()] = false;
    
            break;        
        }
    }
}

bool Application::Init()
{
    glfwInit();
    m_pWindow = glfwCreateWindow(1280, 720, "No title", nullptr, nullptr);

    glfwSetWindowCloseCallback(m_pWindow, GlfwWindowCloseCallback);

    glfwSetCursorPosCallback(m_pWindow, GlfwCursorPosCallback);
    glfwSetMouseButtonCallback(m_pWindow, GlfwMouseButtonCallback);

    glfwSetKeyCallback(m_pWindow, GlfwKeyCallback);

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(0);

    gladLoadGL();
    if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 6)) { return false; }

    m_EventManager.AddListener(AppQuitEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));

    m_EventManager.AddListener(MouseMoveEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));
    m_EventManager.AddListener(MouseButtonDownEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));
    m_EventManager.AddListener(MouseButtonUpEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));

    m_EventManager.AddListener(KeyDownEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));
    m_EventManager.AddListener(KeyUpEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight(&ImGui::GetStyle());
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18);

    ImGui_ImplGlfw_InitForOpenGL(&g_pApp->GetWindow(), true);
    ImGui_ImplOpenGL3_Init();

    m_Renderer.Init();
    m_Console.Init();

    glCreateBuffers(1, &m_UboId);	
    glNamedBufferData(m_UboId, sizeof(glm::mat4) * 2, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UboId);

    auto mesh = std::make_shared<Mesh>();
    mesh->Init();
    m_Meshes.insert(std::make_pair("Cube", mesh));

    m_bRunning = true;
    
    return true;
}

void Application::Run()
{
    float lastTime = (float)glfwGetTime();

    while (m_bRunning)
    {
        float time = (float)glfwGetTime();
        float deltaTime = time - lastTime;

        lastTime = time;

        glfwPollEvents();

        glm::vec2 deltaMousePos = m_MousePos - m_LastMousePos;
        m_LastMousePos = m_MousePos;

        if (m_bRightMouseButtonDown)
        {
            if (!(deltaMousePos.x == 0 && deltaMousePos.y == 0))
            {
                const float rotationSpeed = 0.085f;
                m_Yaw += deltaMousePos.x * rotationSpeed;
                m_Pitch += -deltaMousePos.y * rotationSpeed;

                glm::vec3 dir;
                dir.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                dir.y = sinf(glm::radians(m_Pitch));
                dir.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                
                m_Dir = glm::normalize(dir);
            }

            float speed = 10.5f;
            glm::vec3 right = glm::cross(m_Dir, m_Up);

            if (m_bKeys[GLFW_KEY_W])
            {
                m_Pos += m_Dir * speed * deltaTime;
            }
            else if (m_bKeys[GLFW_KEY_S])
            {
                m_Pos -= m_Dir * speed * deltaTime;
            }

            if (m_bKeys[GLFW_KEY_A])
            {
                m_Pos -= right * speed * deltaTime;
            }
            else if (m_bKeys[GLFW_KEY_D])
            {
                m_Pos += right * speed * deltaTime;
            }

            if (m_bKeys[GLFW_KEY_Q])
            {
                m_Pos += m_Up * speed * deltaTime;
            }
            else if (m_bKeys[GLFW_KEY_E])
            {
                m_Pos -= m_Up * speed * deltaTime;
            }
        }

        int width, height;
        glfwGetFramebufferSize(m_pWindow, &width, &height);
        glViewport(0, 0, width, height);

        glm::mat4 view = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);

        glNamedBufferSubData(m_UboId, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glNamedBufferSubData(m_UboId, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));

        m_Renderer.Draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        m_Console.Draw();

        ImGui::Begin("Actors");
        ImGui::BeginChild("Actors.LeftPane", ImVec2(150, 0),  ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
        
        for (auto itr = m_Actors.begin(); itr != m_Actors.end(); itr++)
        {
            std::string s = "Actor (" + std::to_string((*itr)->GetId()) + ")";
            ImGui::Selectable(s.c_str());
        }
        
        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
    }
}

std::shared_ptr<Mesh> Application::GetMesh(const std::string& asset)
{
    auto findItr = m_Meshes.find(asset);
    if (findItr != m_Meshes.end())
    {
        return findItr->second;
    }
    
    return std::shared_ptr<Mesh>();
}

void Application::CreateActor()
{
    auto actor = std::make_shared<Actor>(m_LastActorId++);
    actor->Init();

    m_Actors.push_back(actor);

    auto transformComponent = std::make_shared<TransformComponent>();
    transformComponent->SetOwner(actor);
    actor->AddComponent(transformComponent);
    
    auto meshDrawComponent = std::make_shared<MeshDrawComponent>();
    meshDrawComponent->SetOwner(actor);
    meshDrawComponent->SetMeshAsset("Cube");
    actor->AddComponent(meshDrawComponent);
    m_EventManager.TriggerEvent(std::make_shared<CreateMeshDrawComponentEvent>(actor->GetId()));
}

void Application::DestroyActor(ActorId id)
{
    m_EventManager.TriggerEvent(std::make_shared<DestroyMeshDrawComponentEvent>(id));

    for (auto itr = m_Actors.begin(); itr != m_Actors.end(); itr++)
    {
        if ((*itr)->GetId() != id) { continue; }
        
        if (m_Actors.size() > 1) 
        { 
            std::swap(m_Actors.back(), (*itr)); 
        }
        
        m_Actors.pop_back();
        break;
    }
}

std::weak_ptr<Actor> Application::GetActor(ActorId id)
{
    for (auto itr = m_Actors.begin(); itr != m_Actors.end(); itr++)
    {
        if ((*itr)->GetId() != id) { continue; }
        return std::weak_ptr(*itr);
    }

    return std::weak_ptr<Actor>();
}