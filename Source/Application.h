#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

#include "EventManager.h"

//-----------------------------------------------------------------------------
// class Console
//-----------------------------------------------------------------------------

class Console
{
public:
    Console();
    ~Console();

    bool Init();
    void Draw();

private:
    std::vector<std::string> m_prevTexts;
    bool m_bScroll;
};

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    void OnEvent(std::shared_ptr<Event> event);

    bool Init();
    void Run();

    GLFWwindow* GetWindow() { return m_pWindow; }

    EventManager* GetEventManager() { return &m_eventManager; }

private:
    bool m_bRunning;

    GLFWwindow* m_pWindow;

    EventManager m_eventManager;

    Console m_console;
};

extern Application* g_pApp;