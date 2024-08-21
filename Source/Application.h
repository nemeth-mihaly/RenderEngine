#pragma once

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

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
};