#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "Scene.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Lights.h"
#include "Mesh.h"

extern StrongTexturePtr g_UvGridTexture;
extern StrongTexturePtr g_SphereGlowTexture;
extern StrongTexturePtr g_DirtTexture;

extern StrongMeshPtr g_MonkeyMesh;

extern StrongShaderPtr g_TexturedLitShader;
extern StrongShaderPtr g_SkyShader;
extern StrongShaderPtr g_BillboardShader;

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
    bool    m_bRunning;

    SDL_Window*     m_pWindow;
    SDL_GLContext   m_pContext;

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