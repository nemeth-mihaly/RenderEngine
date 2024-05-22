#pragma once

#include "Common.h"
#include "Core/App.h"

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
    
    AssetManager* m_pAssetManager;

    void QuitApp();
};

extern Engine* g_pEngine;