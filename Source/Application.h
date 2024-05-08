#pragma once

#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

#include "glad/glad_wgl.h"
#include "glad/glad.h"

#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Win32 programming in C++:
//      https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program
//      ˘˘˘

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

    bool m_bKeyStates[256];

    HDC m_hDeviceContext;
    HGLRC m_hGLRenderContext;

    GLuint m_VertexArray;

    std::vector<GLfloat> m_Vertices;
    GLuint m_VertexBuffer;

    GLuint m_Program;

    GLuint m_StonebricksTexture;

    glm::vec3 m_MeshPosition;

    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;

    float m_Yaw;
    float m_Pitch;

    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraForwardDir;

    glm::mat4 m_View;
    glm::mat4 m_Projection;
};