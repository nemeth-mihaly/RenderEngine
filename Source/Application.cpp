#include "Application.h"

#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

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
}

Application::~Application()
{
    glDeleteShader(m_ShaderId);

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
            if (evt->GetKeyId() == GLFW_KEY_ESCAPE)
            {
                m_bRunning = false;
            }
            
            break;            
        }

        case KeyUpEvent::s_Type:
        {
            auto evt = std::static_pointer_cast<KeyUpEvent>(event);
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

    m_Actor.Init();

    m_bRunning = true;
    
    return true;
}

void Application::Run()
{
    while (m_bRunning)
    {
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(m_pWindow, &width, &height);

        glViewport(0, 0, width, height);

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_ShaderId);

        m_Actor.Draw();

        glUseProgram(0);

        glfwSwapBuffers(m_pWindow);
    }
}