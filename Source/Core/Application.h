#pragma once

#include "Common.h"

#include <memory>
#include <chrono>
#include <string>
#include <vector>

#include "Graphics3D/Buffer.h"
#include "Graphics3D/VertexArray.h"
#include "Graphics3D/ShaderProgram.h"
#include "Graphics3D/Texture.h"

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

    std::vector<Vertex_Position> m_UVSphereVertices;
    std::vector<Vertex_Textured> m_TriangleVertices;
    std::vector<Vertex_Textured> m_RectangleVertices;

    StrongBufferPtr m_VertexBuffer_Position;
    StrongVertexArrayPtr m_VertexArray_Position;

    StrongBufferPtr m_VertexBuffer_Textured;
    StrongVertexArrayPtr m_VertexArray_Textured;

    //GLuint m_VertexArray_Textured;
    //GLuint m_VertexBuffer_Textured;

    StrongTexturePtr m_Texture_Stonebricks;

    glm::mat4 m_View;
    glm::mat4 m_Projection;

    glm::vec3 m_CubeAPosition;
    glm::vec3 m_TriangleAPosition;
    glm::vec3 m_RectangleAPosition;

    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraForwardDir;

    bool m_bKeyStates[256];
    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;
    bool m_bWireframeEnabled;

    float m_Yaw;
    float m_Pitch;
};