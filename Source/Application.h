#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "Scene.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Lights.h"
#include "Mesh.h"

#include "Common.h"

extern Texture_t* g_UvGridTexture;
extern Texture_t* g_SphereGlowTexture;

extern Mesh_t* g_MonkeyMesh;

extern Shader_t* g_TexturedLitShader;
extern Shader_t* g_SkyShader;
extern Shader_t* g_BillboardShader;

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

 private:
    void ProcessEvents();

 private:
    bool m_bRunning;

    SDL_Window*     m_pWindow;
    SDL_GLContext   m_pContext;

    bool        m_bKeyStates[SDL_NUM_SCANCODES];
    glm::vec2   m_CurrentMousePos;
    glm::vec2   m_PrevMousePos;

    Scene* m_pScene;

    bool    m_bCameraMoving;
    float   m_Yaw;
    float   m_Pitch;
};

extern Application* g_pApp;