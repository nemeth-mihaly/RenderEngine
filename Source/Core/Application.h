#pragma once

#include "Common.h"

#include <string>
#include <vector>

#include "AssetManager.h"
#include "Graphics3D/API/VertexBuffer.h"
#include "Graphics3D/API/VertexArray.h"
#include "Graphics3D/API/ProgramPipeline.h"
#include "Graphics3D/API/Texture.h"
#include "Graphics3D/Geometry.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/SceneNodes.h"
#include "Graphics3D/Light.h"

////////////////////////////////////////////////////
//  class Application
////////////////////////////////////////////////////

class Application
{
    friend void GlfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
    friend void GlfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    friend void GlfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

 public:
    Application();
    ~Application();

    bool Initialize();
    void Run();

    AssetManager& GetAssetManager() { return *m_AssetManager; }

 private:
    GLFWwindow* m_pWindow;

    bool m_bIsRunning;

    AssetManager* m_AssetManager;

    StrongMeshPtr m_Mesh_Rectangle;
    StrongMeshPtr m_Mesh_Cube;
    StrongMeshPtr m_Mesh_Terrain;

    StrongTexturePtr m_Texture_Sky;

    SceneNodeList m_SceneNodes;
    AlphaSceneList m_AlphaSceneNodes;
    LightSceneNodeList m_LightNodes;
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<LightNode> m_DirectionalLightNode;
    std::shared_ptr<LightNode> m_PointLightNode;
    std::shared_ptr<LightNode> m_SpotLightNode;

    GLuint m_FramebufferID;
    GLuint m_TextureID_ColorBuffer;
    GLuint m_RenderBufferID;
    StrongTexturePtr m_Texture_ColorBuffer;

    bool m_bKeyStates[256];
    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;

    float m_Yaw;
    float m_Pitch;
};

extern Application* g_pApp;