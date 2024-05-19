#pragma once

#include "Common.h"

#include <memory>
#include <chrono>
#include <string>
#include <vector>

#include "AssetManager.h"
#include "Graphics3D/Buffer.h"
#include "Graphics3D/VertexArray.h"
#include "Graphics3D/Geometry.h"
#include "Graphics3D/ShaderProgram.h"
#include "Graphics3D/Texture.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/SceneNodes.h"

////////////////////////////////////////////////////
//  class Application
////////////////////////////////////////////////////

class Application
{
    friend LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

 public:
    Application(HINSTANCE hInstance);
    ~Application();

    bool Init();
    void MainLoop();

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    AssetManager& GetAssetManager() { return *m_AssetManager; }

 private:
    void InitWindow();
    void InitOpenGL();

 private:
    int m_ScreenWidth;
    int m_ScreenHeight;

    HINSTANCE m_hInstance;
    HWND m_hWindow;

    HDC m_hDeviceContext;
    HGLRC m_hGLRenderContext;

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