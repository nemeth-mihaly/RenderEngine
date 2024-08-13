#pragma once

#include <cstddef>
#include <cstdint>

#include "3rdParty/GLFW/glfw3.h"

//-----------------------------------------------------------------------------
// Forward declarations & typedefs
//-----------------------------------------------------------------------------

class Application;

extern Application* g_pApp;

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    bool Init();
    
    void Run();

private:
    
};