#pragma once

#include "Common.h"
#include "Core/App.h"
#include "Resources/ResourceManager.h"

////////////////////////////////////////////////////
//  class Engine
////////////////////////////////////////////////////

class Engine
{
    bool m_bRunning;

    GLFWwindow* m_pWindow;

 public:
    Engine();
    ~Engine();

    bool Init();
    void Run();

    App* m_pApp;
    

    ResourceManager& GetResourceManager() const { return *(m_pResManager); }

    void QuitApp();

    void ShowCursor();
    void HideCursor();

 private:
    ResourceManager* m_pResManager;
};

extern Engine* g_pEngine;