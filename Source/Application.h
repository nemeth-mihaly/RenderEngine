#pragma once

#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "EventManager.h"

#include "Actor.h"

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    void OnEvent(std::shared_ptr<IEvent> event);

    bool Initialize();
    void Run();

    GLFWwindow& GetWindow() { return *m_pWindow; }
    EventManager& GetEventManager() { return m_EventManager; }

private:
    bool            m_bRunning;

    GLFWwindow*     m_pWindow;

    EventManager    m_EventManager;

    uint32_t        m_ShaderId;

    Actor           m_Actor;
};

extern Application* g_pApp;