#include "Application.h"

#include <cstdio>
#include <string.h>
#include <ctype.h>

#include "Events.h"

Application* g_pApp = nullptr;

//-----------------------------------------------------------------------------
// Console Implementation
//-----------------------------------------------------------------------------

Console::Console()
{
    m_bScroll = false;
}

Console::~Console()
{
    m_prevTexts.clear();
}

bool Console::Init()
{
    return true;
}

void Console::Draw()
{
    ImGui::Begin("Console");

    float height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("Console.ScrollingRegion", ImVec2(0, -height), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar);

    for (auto itr = m_prevTexts.begin(); itr != m_prevTexts.end(); itr++)
    {
        ImGui::TextUnformatted((*itr).c_str());
    }
    
    if (m_bScroll) 
    {
        ImGui::SetScrollHereY(1.0f);
        m_bScroll = false;
    }
    
    ImGui::EndChild();

    ImGui::Separator();

    char buf[128];
    memset(buf, 0, sizeof(buf));

    bool bReclaimfocus = false;
    if (ImGui::InputText("Console.InputText", buf, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
    {
        std::string s(buf);
        m_prevTexts.push_back(s);

        if (stricmp(s.c_str(), "CLEAR") == 0)
        {
            m_prevTexts.clear();
        }
        else if (stricmp(s.c_str(), "QUIT") == 0)
        {
            g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_CloseWindow>());
        }
        else
        {
            m_prevTexts.push_back("Unknown command '" + s + "'");
        }

        m_bScroll = true;
        bReclaimfocus = true;
    }

    ImGui::SetItemDefaultFocus();

    if (bReclaimfocus)
    {
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
    }

    ImGui::End();
}

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

static void GlfwWindowCloseCallback(GLFWwindow* window) { g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_CloseWindow>()); }

static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_KeyDown>(key, mods));
    }
    else if (action == GLFW_RELEASE)
    {
        g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_KeyUp>(key, mods));
    }
}

Application::Application()
{
    g_pApp = this;

    m_bRunning = false;

    m_pWindow = nullptr;
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_pWindow) { glfwDestroyWindow(m_pWindow); }
    glfwTerminate();
}

void Application::OnEvent(std::shared_ptr<Event> event)
{
    switch (event->GetType())
    {
        case Event_CloseWindow::s_type:
        {
            m_bRunning = false;
            break;
        }

        case Event_KeyDown::s_type:
        {
            auto castevt = std::static_pointer_cast<Event_KeyDown>(event);
            break;
        }

        case Event_KeyUp::s_type:
        {
            auto castevt = std::static_pointer_cast<Event_KeyUp>(event);
            break;
        }
    }
}

bool Application::Init()
{
    glfwInit();
    m_pWindow = glfwCreateWindow(1280, 720, "MyRenderEngine", nullptr, nullptr);

    glfwSetWindowCloseCallback(m_pWindow, GlfwWindowCloseCallback);
    glfwSetKeyCallback(m_pWindow, GlfwKeyCallback);

    glfwMakeContextCurrent(m_pWindow);

    glfwSwapInterval(0);

    m_eventManager.AddListener(BIND_EVENT_FN(&Application::OnEvent, this), Event_CloseWindow::s_type);
    m_eventManager.AddListener(BIND_EVENT_FN(&Application::OnEvent, this), Event_KeyDown::s_type);
    m_eventManager.AddListener(BIND_EVENT_FN(&Application::OnEvent, this), Event_KeyUp::s_type);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight(&ImGui::GetStyle());
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18);

    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    ImGui_ImplOpenGL3_Init();

    m_console.Init();

    m_bRunning = true;
    return true;
}

void Application::Run()
{
    while (m_bRunning)
    {
        glfwPollEvents();

        glClearColor(0.15f, 0.15f, 0.15f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        m_console.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
    }
}