#include "Application.h"

#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

Application* g_pApp = nullptr;

static void GlfwWindowCloseCallback([[maybe_unused]] GLFWwindow* pWindow) { g_pApp->GetEventManager().TriggerEvent(std::make_shared<AppQuitEvent>()); }

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

    memset(m_bKeys, 0x00, sizeof(m_bKeys));

    m_LastActorId = 0;
}

Application::~Application()
{
    glDeleteShader(m_ShaderId);

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

bool Application::Initialize()
{
    glfwInit();
    m_pWindow = glfwCreateWindow(1280, 720, "No title", nullptr, nullptr);

    glfwSetWindowCloseCallback(m_pWindow, GlfwWindowCloseCallback);
    glfwSetKeyCallback(m_pWindow, GlfwKeyCallback);

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(0);

    gladLoadGL();
    if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 6)) { return false; }

    m_EventManager.AddListener(AppQuitEvent::s_Type, std::bind(&Application::OnEvent, this, std::placeholders::_1));

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

    m_Console.Initialize();

    uint32_t vsId, fsId;
    m_ShaderId = glCreateProgram();

    {
        std::ifstream file("Assets/Shaders/Shader.vert", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        vsId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsId, 1, &pSource, nullptr);
        glCompileShader(vsId);

        delete[] pSource;
    }

    {
        std::ifstream file("Assets/Shaders/Shader.frag", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        fsId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsId, 1, &pSource, nullptr);
        glCompileShader(fsId);

        delete[] pSource;
    }

    glAttachShader(m_ShaderId, vsId);
    glAttachShader(m_ShaderId, fsId);
    glLinkProgram(m_ShaderId);

    glDeleteShader(vsId);
    glDeleteShader(fsId);

    {
        auto actor = std::make_shared<Actor>(m_LastActorId++);
        actor->Init();

        actor->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        m_Actors.push_back(actor);
    }

    {
        auto actor = std::make_shared<Actor>(m_LastActorId++);
        actor->Init();

        actor->SetPosition(glm::vec3(-1.2f, 2.0f, 0.5f));

        m_Actors.push_back(actor);
    }

    {
        auto actor = std::make_shared<Actor>(m_LastActorId++);
        actor->Init();

        actor->SetPosition(glm::vec3(-0.2f,-2.5f,-3.0f));

        m_Actors.push_back(actor);
    }

    m_bRunning = true;
    
    return true;
}

void Application::Run()
{
    float lastTime = (float)glfwGetTime();

    while (m_bRunning)
    {
        float time = (float)glfwGetTime();
        float dt = time - lastTime;

        lastTime = time;

        glfwPollEvents();

        float speed = 10.5f;
        glm::vec3 right = glm::cross(m_Dir, m_Up);

        if (m_bKeys[GLFW_KEY_W])
        {
            m_Pos += m_Dir * speed * dt;
        }
        else if (m_bKeys[GLFW_KEY_S])
        {
            m_Pos -= m_Dir * speed * dt;
        }

        if (m_bKeys[GLFW_KEY_A])
        {
            m_Pos -= right * speed * dt;
        }
        else if (m_bKeys[GLFW_KEY_D])
        {
            m_Pos += right * speed * dt;
        }

        if (m_bKeys[GLFW_KEY_Q])
        {
            m_Pos += m_Up * speed * dt;
        }
        else if (m_bKeys[GLFW_KEY_E])
        {
            m_Pos -= m_Up * speed * dt;
        }

        int width, height;
        glfwGetFramebufferSize(m_pWindow, &width, &height);

        glViewport(0, 0, width, height);

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_ShaderId);

        glm::mat4 view = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, "uView"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, "uProj"), 1, GL_FALSE, glm::value_ptr(proj));

        // Draw 'm_Actors'
        for (auto itr = m_Actors.begin(); itr != m_Actors.end(); itr++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), (*itr)->GetPosition());
            glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

            (*itr)->Draw(); 
        }

        glUseProgram(0);

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

void Application::CreateActor()
{
    m_Actors.push_back(std::make_shared<Actor>(m_LastActorId++));
}

void Application::DestroyActor(ActorId id)
{
    for (auto itr = m_Actors.begin(); itr != m_Actors.end(); itr++)
    {
        if ((*itr)->GetId() != id) { continue; }
        
        if (m_Actors.size() > 1) 
        { 
            std::swap(m_Actors.back(), (*itr)); 
        }
        
        m_Actors.pop_back();
        return;
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