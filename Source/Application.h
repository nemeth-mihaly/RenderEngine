#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "EventManager.h"

#include "Console.h"

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

    void CreateActor();
    void DestroyActor(ActorId id);
    std::weak_ptr<Actor> GetActor(ActorId id);

private:
    bool            m_bRunning;

    GLFWwindow*     m_pWindow;

    EventManager    m_EventManager;

    glm::vec2       m_MousePos;
    glm::vec2       m_LastMousePos;
    bool            m_bRightMouseButtonDown;
    bool            m_bKeys[GLFW_KEY_LAST];

    Console         m_Console;

    uint32_t        m_ShaderId;

    ActorId         m_LastActorId;
    std::vector<std::shared_ptr<Actor>> m_Actors;

    float           m_Yaw = -90.0f;
    float           m_Pitch = 0.0f;
    glm::vec3       m_Pos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3       m_Dir = glm::vec3(0.0f, 0.0f,-1.0f);
    glm::vec3       m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
};

extern Application* g_pApp;