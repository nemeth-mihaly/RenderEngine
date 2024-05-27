#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "VertexArray.h"
#include "Lights.h"
#include "Mesh.h"

#include "Common.h"
#include "Resources/ResourceManager.h"
#include "Graphics3D/SceneNodes.h"

extern std::shared_ptr<Texture_t> g_UvGridTexture;

extern std::shared_ptr<Mesh_t> g_MonkeyMesh;

////////////////////////////////////////////////////
//  class App
////////////////////////////////////////////////////

class App
{
 public:
    App();
    ~App();

    bool Init();
    void RunLoop();    

    ResourceManager& GetResourceManager() const { return *(m_pResManager); }

    const std::shared_ptr<CameraSceneNode>& GetCamera() const { return m_CameraSceneNode; }

 private:
    void CreateScene();
    void UpdateScene(const float deltaTime);
    void RenderScene();

    bool m_bRunning;

    SDL_Window* m_pWindow;
    SDL_GLContext m_pContext;

    bool m_bKeyStates[SDL_NUM_SCANCODES];

    ResourceManager* m_pResManager;

    bool m_bCameraMoving;
    glm::vec2 m_CurrentMousePos;
    glm::vec2 m_PrevMousePos;
    float m_Yaw;
    float m_Pitch;

    SceneNodeList_t m_SceneNodes;
    AlphaSceneList m_AlphaSceneNodes;
    LightSceneNodeList_t m_LightNodes;
    std::shared_ptr<CameraSceneNode> m_CameraSceneNode;
    std::shared_ptr<SkySceneNode_t> SkySceneNode;
};

extern App* g_pApp;