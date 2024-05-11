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

    StrongMeshPtr m_Mesh_Triangle;
    StrongMeshPtr m_Mesh_Rectangle;
    StrongMeshPtr m_Mesh_Cube;

    StrongBufferPtr m_VertexBuffer_Multiple;
    StrongVertexArrayPtr m_VertexArray_Multiple;

    StrongBufferPtr m_VertexBuffer;
    StrongVertexArrayPtr m_VertexArray;

    StrongTexturePtr m_Texture_Stonebricks;

    std::shared_ptr<SceneNode> m_SceneNode1;
    std::shared_ptr<SceneNode> m_SceneNode2;
    std::shared_ptr<SceneNode> m_SceneNode3;

    std::shared_ptr<CameraNode> m_Camera;

    bool m_bKeyStates[256];
    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;
    bool m_bWireframeEnabled;

    float m_Yaw;
    float m_Pitch;
};