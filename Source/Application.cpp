#include "Application.h"

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

        glfwSwapBuffers(m_pWindow);
    }
}