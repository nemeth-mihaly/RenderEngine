#pragma once

#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "Actor.h"

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();

    GLFWwindow& GetWindow() { return *m_pWindow; }

private:
    bool            m_bRunning;

    GLFWwindow*     m_pWindow;

    Actor           m_actor;
    
    /*
    int             m_NumVerts;
    uint32_t        m_VaoId;
    uint32_t        m_VboId;
    */
};