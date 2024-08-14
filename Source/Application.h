#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

#include "EventManager.h"

#include "Actor.h"

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

    void SetVisible(bool bVisible) { m_bVisible = bVisible; }
    bool IsVisible() const { return m_bVisible; }

private:
    bool m_bVisible;
    bool m_bScroll;

    std::vector<std::string> m_prevTexts;
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

    void OnActorCreate(std::shared_ptr<Event> event);
    void OnActorDestroy(std::shared_ptr<Event> event);

private:
    bool m_bRunning;

    GLFWwindow* m_pWindow;

    EventManager m_eventManager;

    Console m_console;

    std::vector<std::shared_ptr<Actor>> m_actors;
};

extern Application* g_pApp;