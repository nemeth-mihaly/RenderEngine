#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "Common.h"
#include "Resources/ResourceManager.h"
#include "Graphics3D/SceneNodes.h"
#include "Graphics3D/Light.h"

////////////////////////////////////////////////////
//  class Application
////////////////////////////////////////////////////

class Application
{
    friend void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void CursorPositionCallback(GLFWwindow* window, double x, double y);
    friend void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

 public:
    Application();
    ~Application();

    bool Init();
    void Run();    

    ResourceManager& GetResourceManager() const { return *(m_pResManager); }

 private:
    void CreateScene();
    void UpdateScene(const float deltaTime);
    void RenderScene();

 private:
    bool m_bRunning;

    GLFWwindow* m_pWindow;

    ResourceManager* m_pResManager;

    bool m_bKeyStates[256];
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
    std::shared_ptr<LightNode> m_DirectionalLightNode;
    std::shared_ptr<LightNode> m_PointLightNode;
    std::shared_ptr<LightNode> m_SpotLightNode;

    GLuint m_FramebufferID;
    GLuint m_TextureID_ColorBuffer;
    GLuint m_RenderBufferID;
    StrongTexturePtr m_Texture_ColorBuffer;
};

extern Application* g_pApp;