#include "Application.h"

#include <cstdio>
#include <cstring>
#include <cctype>

#include "Events.h"

Application* g_pApp = nullptr;

//-----------------------------------------------------------------------------
// Console Implementation
//-----------------------------------------------------------------------------

Console::Console()
{
    m_bVisible = false;
    m_bScroll = false;
}

Console::~Console()
{
    m_prevTexts.clear();
}

bool Console::Init()
{
    m_prevTexts.push_back("Enter 'HELP' for help.");

    return true;
}

void Console::Draw()
{
    if (!IsVisible())
    {
        return;
    }

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

        std::vector<std::string> tokens;

        int pos = 0;
        while (pos < s.size())
        {
            pos = s.find(' ');
            std::string token = s.substr(0, pos);
            tokens.push_back(token);
            s.erase(0, pos + 1);
        }

        tokens.push_back(s);

        if (stricmp(tokens[0].c_str(), "HELP") == 0)
        {   
            
        }
        else if (stricmp(tokens[0].c_str(), "CLEAR") == 0)
        {
            m_prevTexts.clear();
        }
        else if (stricmp(tokens[0].c_str(), "QUIT") == 0)
        {
            g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_CloseWindow>());
        }
        else if (stricmp(tokens[0].c_str(), "ACTORCREATE") == 0)
        {
            ActorId id = (ActorId)atoi(tokens[1].c_str());
            g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_ActorCreate>(id));
        }
        else if (stricmp(tokens[0].c_str(), "ACTORDESTROY") == 0)
        {
            ActorId id = (ActorId)atoi(tokens[1].c_str());
            g_pApp->GetEventManager()->TriggerEvent(std::make_shared<Event_ActorDestroy>(id));
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
            auto evt = std::static_pointer_cast<Event_KeyDown>(event);
            if (evt->GetKeyId() == GLFW_KEY_GRAVE_ACCENT)
            {
                bool bActive = !m_console.IsVisible();
                m_console.SetVisible(bActive);
            }

            break;
        }

        case Event_KeyUp::s_type:
        {
            auto evt = std::static_pointer_cast<Event_KeyUp>(event);
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

    m_eventManager.AddListener(BIND_EVENT_FN(&Application::OnActorCreate, this), Event_ActorCreate::s_type);
    m_eventManager.AddListener(BIND_EVENT_FN(&Application::OnActorDestroy, this), Event_ActorDestroy::s_type);

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

        ImGui::Begin("Actors");
        ImGui::BeginChild("Actors.LeftPane", ImVec2(150, 0),  ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
        
        for (auto itr = m_actors.begin(); itr != m_actors.end(); itr++)
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

void Application::OnActorCreate(std::shared_ptr<Event> event)
{
    auto evt = std::static_pointer_cast<Event_ActorCreate>(event);
    m_actors.push_back(std::make_shared<Actor>(evt->GetActorId()));
}

void Application::OnActorDestroy(std::shared_ptr<Event> event)
{
    auto evt = std::static_pointer_cast<Event_ActorDestroy>(event);
    for (auto itr = m_actors.begin(); itr != m_actors.end(); itr++)
    {
        if ((*itr)->GetId() == evt->GetActorId())
        {
            std::swap(m_actors.back(), (*itr));
            m_actors.pop_back();

            return;
        }
    }
}