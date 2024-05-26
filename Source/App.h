#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "Common.h"
#include "Resources/ResourceManager.h"
#include "Graphics3D/SceneNodes.h"
#include "Graphics3D/Light.h"

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

    StrongMeshPtr m_Mesh_Rectangle;
    StrongMeshPtr m_Mesh_Cube;
    StrongMeshPtr m_Mesh_Terrain;

    StrongTexturePtr m_Texture_Sky;

    SceneNodeList m_SceneNodes;
    AlphaSceneList m_AlphaSceneNodes;
    LightSceneNodeList m_LightNodes;
    std::shared_ptr<CameraSceneNode> m_CameraSceneNode;

    GLuint m_FramebufferID;
    GLuint m_TextureID_ColorBuffer;
    GLuint m_RenderBufferID;
    StrongTexturePtr m_Texture_ColorBuffer;
};

extern App* g_pApp;