#include "Application.h"

#include <cstdio>
#include <cassert>

const wchar_t ClassName[] = TEXT("MyWindow");

// Window procedure wrapped in C++ class:
//      https://devblogs.microsoft.com/oldnewthing/20191014-00/?p=102992
//      ˘˘˘

LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Application* pApp = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pApp = static_cast<Application*>(lpcs->lpCreateParams);
        SetWindowLongPtr(hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));

        pApp->m_hWindow = hWindow;
    }
    else
    {
        pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWindow, GWLP_USERDATA));
    }

    if (pApp)
    {
        return pApp->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hWindow, uMsg, wParam, lParam);
}

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
    m_hWindow = nullptr;

    m_hDeviceContext = nullptr;
    m_hGLRenderContext = nullptr;
}

Application::~Application()
{
    glDeleteProgram(m_Program);

    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteVertexArrays(1, &m_VertexBuffer);

    wglDeleteContext(m_hGLRenderContext);
    ReleaseDC(m_hWindow, m_hDeviceContext);

    DestroyWindow(m_hWindow);
}

bool Application::Init()
{
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = m_hInstance;
    wcex.lpszClassName = ClassName;

    ATOM wcexClassID = RegisterClassEx(&wcex);
    assert(wcexClassID);

    m_hWindow = CreateWindowEx(
        0,
        ClassName,
        TEXT("Engine"),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    assert(m_hWindow);

    // Proper OpenGL context creation:
    //      https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
    //      ˘˘˘

    // Temp context:

    WNDCLASSEX temp_wcex;
    memset(&temp_wcex, 0, sizeof(temp_wcex));

    temp_wcex.cbSize = sizeof(WNDCLASSEX);
    temp_wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    temp_wcex.lpfnWndProc = DefWindowProc;
    temp_wcex.hInstance = m_hInstance;
    temp_wcex.lpszClassName = TEXT("temp_ClassName");

    ATOM temp_wcexClassID = RegisterClassEx(&temp_wcex);
    assert(temp_wcexClassID);

    HWND temp_hWindow = CreateWindowEx(
        0,
        TEXT("temp_ClassName"),
        TEXT("temp"),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    
        nullptr,
        nullptr,
        m_hInstance,
        nullptr
    );

    HDC temp_hDeviceContext = GetDC(temp_hWindow);
    assert(temp_hDeviceContext);

    PIXELFORMATDESCRIPTOR temp_pixelformatDescriptor;
    memset(&temp_pixelformatDescriptor, 0, sizeof(temp_pixelformatDescriptor));

    temp_pixelformatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    temp_pixelformatDescriptor.nVersion = 1;
    temp_pixelformatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    temp_pixelformatDescriptor.iPixelType = PFD_TYPE_RGBA;
    temp_pixelformatDescriptor.cColorBits = 32;
    temp_pixelformatDescriptor.cAlphaBits = 8;
    temp_pixelformatDescriptor.cDepthBits = 24;
    temp_pixelformatDescriptor.cStencilBits = 8;
    temp_pixelformatDescriptor.iLayerType = PFD_MAIN_PLANE;

    int temp_pixelformat = ChoosePixelFormat(temp_hDeviceContext, &temp_pixelformatDescriptor);
    SetPixelFormat(temp_hDeviceContext, temp_pixelformat, &temp_pixelformatDescriptor);

    HGLRC temp_hGLRenderContext = wglCreateContext(temp_hDeviceContext);
    assert(temp_hGLRenderContext);

    WINBOOL bResult = wglMakeCurrent(temp_hDeviceContext, temp_hGLRenderContext);
    assert(bResult);

    bResult = gladLoadWGL(temp_hDeviceContext);
    assert(bResult);

    bResult = gladLoadGL();
    assert(bResult);

    wglDeleteContext(temp_hGLRenderContext);
    ReleaseDC(temp_hWindow, temp_hDeviceContext);

    DestroyWindow(temp_hWindow);

    // Final context:

    m_hDeviceContext = GetDC(m_hWindow);
    assert(m_hDeviceContext);

    const int choosePixelformatAttribs[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End
    };

    int pixelformat;
    UINT pixelformatCount;
    bResult = wglChoosePixelFormatARB(m_hDeviceContext, choosePixelformatAttribs, nullptr, 1, &pixelformat, &pixelformatCount);
    assert(bResult);

    PIXELFORMATDESCRIPTOR pixelformatDescriptor;
    DescribePixelFormat(m_hDeviceContext, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), &pixelformatDescriptor);
    SetPixelFormat(m_hDeviceContext, pixelformat, &pixelformatDescriptor);

    int createContextAttribs[] =
    {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
    };

    m_hGLRenderContext = wglCreateContextAttribsARB(m_hDeviceContext, nullptr, createContextAttribs);
    assert(m_hGLRenderContext);

    bResult = wglMakeCurrent(m_hDeviceContext, m_hGLRenderContext);
    assert(bResult);

    printf("OpenGL Version: %s \n", glGetString(GL_VERSION));

    ShowWindow(m_hWindow, SW_SHOW);

    glCreateVertexArrays(1, &m_VertexArray);

    /*
           A 
          / \
         /   \ 
        C-- - B
    */

    m_Vertices = 
    {
        // ACB
        // Position         // Color
        0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    glCreateBuffers(1, &m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, sizeof(GLfloat) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, (6 * sizeof(GLfloat)));

    // Position
    glEnableVertexArrayAttrib(m_VertexArray, 0);
    glVertexArrayAttribFormat(m_VertexArray, 0, 3, GL_FLOAT, GL_FALSE, (0 * sizeof(GLfloat)));
    glVertexArrayAttribBinding(m_VertexArray, 0, 0);

    // Color
    glEnableVertexArrayAttrib(m_VertexArray, 1);
    glVertexArrayAttribFormat(m_VertexArray, 1, 3, GL_FLOAT, GL_FALSE, (3 * sizeof(GLfloat)));
    glVertexArrayAttribBinding(m_VertexArray, 1, 0);

    const std::string vsSource =
    {
        "#version 460 core\n"

        "in layout(location = 0) vec3 a_Pos;"
        "in layout(location = 1) vec3 a_Color;"
        
        "out vec3 v_Color;"

        "void main()"
        "{"
            "gl_Position = vec4(a_Pos, 1.0);"
            "v_Color = a_Color;"
        "}"
    };

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vsSourceCString = vsSource.c_str();
    glShaderSource(vs, 1, &vsSourceCString, nullptr);
    glCompileShader(vs);

    GLint result;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLchar infoLog[BUFSIZ];
        glGetShaderInfoLog(vs, BUFSIZ, nullptr, infoLog);
        printf("%s \n", infoLog);
    }

    const std::string fsSource =
    {
        "#version 460 core\n"

        "in vec3 v_Color;"

        "out vec4 v_FragmentColor;"
        
        "void main()"
        "{"
            "v_FragmentColor = vec4(v_Color, 1.0);"
        "}"
    };

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fsSourceCString = fsSource.c_str();
    glShaderSource(fs, 1, &fsSourceCString, nullptr);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLchar infoLog[BUFSIZ];
        glGetShaderInfoLog(fs, BUFSIZ, nullptr, infoLog);
        printf("%s \n", infoLog);
    }

    m_Program = glCreateProgram();
    
    glAttachShader(m_Program, vs);
    glAttachShader(m_Program, fs);

    glLinkProgram(m_Program);

    glGetShaderiv(m_Program, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLchar infoLog[BUFSIZ];
        glGetProgramInfoLog(m_Program, BUFSIZ, nullptr, infoLog);
        printf("%s \n", infoLog);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void Application::MainLoop()
{
    MSG msg;
    memset(&msg, 0, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(m_Program);

            glBindVertexArray(m_VertexArray);
            glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());

            // TODO: Get WGL_EXT_swap_control.
            wglSwapLayerBuffers(m_hDeviceContext, WGL_SWAP_MAIN_PLANE);
        }
    }
}

LRESULT Application::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }

            return 0;
        };

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(m_hWindow, uMsg, wParam, lParam);
}