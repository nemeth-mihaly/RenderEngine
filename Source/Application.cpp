#include "Application.h"

#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

Application::Application()
{
    m_bRunning = false;

    m_pWindow = nullptr;
}

Application::~Application()
{
    if (m_pWindow) { glfwDestroyWindow(m_pWindow); }
    glfwTerminate();
}

bool Application::Initialize()
{
    glfwInit();
    m_pWindow = glfwCreateWindow(1280, 720, "No title", nullptr, nullptr);

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(0);

    gladLoadGL();
    if (GLVersion.major < 4 || (GLVersion.major == 4 && GLVersion.minor < 6)) { return false; }

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

    m_actor.Init();

    m_bRunning = true;
    
    return true;
}

void Application::Run()
{
    while (m_bRunning && !glfwWindowShouldClose(m_pWindow))
    {
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(m_pWindow, &width, &height);

        glViewport(0, 0, width, height);

        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_ShaderId);

        m_actor.Draw();

        glUseProgram(0);

        glfwSwapBuffers(m_pWindow);
    }
}