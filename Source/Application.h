#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/stb/stb_image.h"
#include "3rdParty/stb/stb_image_write.h"

#include "3rdParty/imgui/imgui.h"
#include "3rdParty/imgui/imgui_impl_sdl2.h"
#include "3rdParty/imgui/imgui_impl_opengl3.h"

#include "Shader.h"
#include "Scene.h"

extern StrongProgramPipelinePtr g_TexturedLitShader;
extern StrongProgramPipelinePtr g_SkyShader;
extern StrongProgramPipelinePtr g_BillboardShader;

////////////////////////////////////////////////////
//  class Application
////////////////////////////////////////////////////

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
    void RunLoop();    

    uint8_t     Colorv;
    int         HeightMapWidth;
    int         HeightMapHeight;
    int         HeightMapChannelCount;
    uint8_t*    pHeightMapData;

private:
    void ProcessEvents();

private:
    bool    m_bRunning;

    SDL_Window*     m_pWindow;
    SDL_GLContext   m_Context;

    bool        m_bKeyStates[SDL_NUM_SCANCODES];
    glm::vec2   m_CurrentMousePos;
    glm::vec2   m_PrevMousePos;

    Scene*      m_pScene;

    bool        m_bDebugCameraEnabled = false;
    bool        m_bCameraMoving;
    float       m_Yaw;
    float       m_Pitch;
};

extern Application* g_pApp;