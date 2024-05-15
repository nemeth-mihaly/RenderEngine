#pragma once

#include "Common.h"

#include <memory>
#include <chrono>
#include <string>
#include <vector>

#include "Graphics3D/Buffer.h"
#include "Graphics3D/VertexArray.h"
#include "Graphics3D/Geometry.h"
#include "Graphics3D/ShaderProgram.h"
#include "Graphics3D/Texture.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/SceneNodes.h"

extern char* _ReadFile(const std::string& name);

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

 private:
    int m_ScreenWidth;
    int m_ScreenHeight;

    HINSTANCE m_hInstance;
    HWND m_hWindow;

    HDC m_hDeviceContext;
    HGLRC m_hGLRenderContext;

    bool m_bIsRunning;

    StrongShaderProgPtr m_ShaderProg_Textured;
    StrongShaderProgPtr m_ShaderProg_Phong;
    StrongShaderProgPtr m_ShaderProg_LightsDbg;
    StrongShaderProgPtr m_ShaderProg_Sky;

    StrongMeshPtr m_Mesh_Rectangle;
    StrongMeshPtr m_Mesh_Cube;

    StrongTexturePtr m_Texture_Stonebricks;
    StrongTexturePtr m_Texture_Grass;
    StrongTexturePtr m_Texture_Sky;

    SceneNodeList m_SceneNodes;
    LightSceneNodeList m_LightNodes;
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<LightNode> m_LightNode;

    bool m_bKeyStates[256];
    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;
    bool m_bWireframeEnabled;

    float m_Yaw;
    float m_Pitch;
};