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
#include "SceneNodes.h"

extern std::shared_ptr<Texture_t> g_UvGridTexture;

extern std::shared_ptr<Mesh_t> g_MonkeyMesh;

extern std::shared_ptr<Shader_t> g_TexturedLitShader;
extern std::shared_ptr<Shader_t> g_SkyShader;

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
    bool m_bRunning;

    SDL_Window* m_pWindow;
    SDL_GLContext m_pContext;

    bool m_bKeyStates[SDL_NUM_SCANCODES];

    Scene* m_pScene;

    bool m_bCameraMoving;
    glm::vec2 m_CurrentMousePos;
    glm::vec2 m_PrevMousePos;
    float m_Yaw;
    float m_Pitch;

    //SceneNodeList_t m_SceneNodes;
    //AlphaSceneList m_AlphaSceneNodes;
    //LightSceneNodeList_t m_LightNodes;
    //std::shared_ptr<CameraSceneNode> m_CameraSceneNode;
    //std::shared_ptr<SkySceneNode_t> m_SkySceneNode;
};

extern Application* g_pApp;