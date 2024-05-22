#include "Core/Engine.h"

Engine* g_pEngine = nullptr;

////////////////////////////////////////////////////
//  Engine Implementation
////////////////////////////////////////////////////

Engine::Engine()
{
    g_pEngine = this;

    m_bRunning = false;
    
    m_pWindow = nullptr;
    
    m_pApp = nullptr;

    m_pAssetManager = nullptr;
}

Engine::~Engine()
{
    if (m_pApp)
        delete m_pApp;

    if (m_pAssetManager)
        delete m_pAssetManager;

    if (m_pWindow)
        glfwDestroyWindow(m_pWindow);

    glfwTerminate();

    if (g_pEngine)
        g_pEngine = nullptr;
}

bool Engine::Init()
{
    m_pAssetManager = new AssetManager();
    if (!m_pAssetManager)
        return false;

    if (!glfwInit())
        return false;

    m_pWindow = glfwCreateWindow(1280, 720, "Render Engine | Testbed", nullptr, nullptr);
    if (!m_pWindow)
        return false;

    glfwSetKeyCallback(m_pWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (!g_pEngine->m_pApp)
            return;

        if (action == GLFW_PRESS)
            g_pEngine->m_pApp->OnKeyDown(key);
        else
        if (action == GLFW_RELEASE)
        {
            g_pEngine->m_pApp->OnKeyUp(key);
        }
    });

    glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow *window, double x, double y) {
        if (!g_pEngine->m_pApp)
            return;

        g_pEngine->m_pApp->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
    });

    glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow *window, int button, int action, int mods) {
        if (!g_pEngine->m_pApp)
            return;

        if (action == GLFW_PRESS)
            g_pEngine->m_pApp->OnMouseButtonDown(button);
        else
        if (action == GLFW_RELEASE)
        {
            g_pEngine->m_pApp->OnMouseButtonUp(button);
        }
    });

    glfwMakeContextCurrent(m_pWindow);
    if (!gladLoadGL())
        return false;

    m_pApp = new App();
    if (!(m_pApp && m_pApp->Init()))
        return false;

    m_bRunning = true;
    return true;
}

void Engine::Run()
{
    float fPreviousTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(m_pWindow) && m_bRunning)
    {
        glfwPollEvents();

        const float fCurrentTime = static_cast<float>(glfwGetTime());
        const float fDeltaTime = fCurrentTime - fPreviousTime;
        fPreviousTime = fCurrentTime;

        m_pApp->UpdateAndRender(fDeltaTime);

        glfwSwapInterval(1);
        glfwSwapBuffers(m_pWindow);
    }
}

void Engine::QuitApp()
{
    m_bRunning = false;
}