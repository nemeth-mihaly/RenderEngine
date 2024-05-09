#include "Application.h"

#include <cstdio>
#include <cassert>

const wchar_t ClassName[] = TEXT("MyWindow");

uint32_t triangleVertexCount;
uint32_t triangleVertexDataByteOffset;
uint32_t rectangleVertexCount;
uint32_t rectangleVertexDataByteOffset;

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
    m_ScreenWidth = 1280;
    m_ScreenHeight = 720;

    m_hWindow = nullptr;

    memset(m_bKeyStates, 0, sizeof(m_bKeyStates));

    m_hDeviceContext = nullptr;
    m_hGLRenderContext = nullptr;

    m_TriangleAPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    m_RectangleAPosition = glm::vec3(5.0f, 0.0f, 0.0f);

    m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    m_CameraForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_bCameraMoving = false;

    m_Yaw = -90.0f;
    m_Pitch = 0.0f;
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

    RECT wr = {0, 0, m_ScreenWidth, m_ScreenHeight};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWindow = CreateWindowEx(
        0,
        ClassName,
        TEXT("Engine"),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, 
        (wr.right - wr.left), 
        (wr.bottom - wr.top),
    
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
    temp_wcex.lpszClassName = TEXT("temp_className");

    ATOM temp_wcexClassID = RegisterClassEx(&temp_wcex);
    assert(temp_wcexClassID);

    HWND temp_hWindow = CreateWindowEx(
        0,
        TEXT("temp_className"),
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
    SetFocus(m_hWindow);

    /*
    //         A 
    //        / \ 
    //       /   \ 
    //      C-- - B
    */

   m_TriangleVertices =
   {
        // Position           // Texcoord

        // ACB
        0.0f, 0.5f, 0.0f,     0.5f, 1.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f
   };

    /*
    //      A --  B
    //      |   / |
    //      | /   |
    //      C  -- D
    */

    m_RectangleVertices =
    {
        // Position             // Texcoord

        // ACB
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f,

        // BCD
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f
    };

    glCreateVertexArrays(1, &m_VertexArray);

    glCreateBuffers(1, &m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, sizeof(GLfloat) * (m_TriangleVertices.size() + m_RectangleVertices.size()), nullptr, GL_STATIC_DRAW);
    glNamedBufferSubData(m_VertexBuffer, 0, sizeof(GLfloat) * m_TriangleVertices.size(), m_TriangleVertices.data());
    glNamedBufferSubData(m_VertexBuffer, sizeof(GLfloat) * m_TriangleVertices.size(), sizeof(GLfloat) * m_RectangleVertices.size(), m_RectangleVertices.data());

    glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, (5 * sizeof(GLfloat)));

    // Pos
    glEnableVertexArrayAttrib(m_VertexArray, 0);
    glVertexArrayAttribFormat(m_VertexArray, 0, 3, GL_FLOAT, GL_FALSE, (0 * sizeof(GLfloat)));
    glVertexArrayAttribBinding(m_VertexArray, 0, 0);

    // Texcoord
    glEnableVertexArrayAttrib(m_VertexArray, 1);
    glVertexArrayAttribFormat(m_VertexArray, 1, 2, GL_FLOAT, GL_FALSE, (3 * sizeof(GLfloat)));
    glVertexArrayAttribBinding(m_VertexArray, 1, 0);

    const std::string vsSource =
    {
        "#version 460 core\n"

        "in layout(location = 0) vec3 a_Pos;"
        "in layout(location = 1) vec2 a_Texcoord;"
        
        "uniform mat4 u_WorldViewProjection;"
        "uniform mat4 u_World;"

        "out vec2 v_Texcoord;"

        "void main()"
        "{"
            "gl_Position = u_WorldViewProjection * u_World * vec4(a_Pos, 1.0);"
            "v_Texcoord = a_Texcoord;"
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
        "out vec4 v_FragmentColor;"

        "in vec2 v_Texcoord;"

        "uniform sampler2D u_Texture;"
        
        "void main()"
        "{"
            //"v_FragmentColor = vec4(v_Color, 1.0);"
            "v_FragmentColor = texture(u_Texture, v_Texcoord);"
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

    stbi_set_flip_vertically_on_load(true);

    int texw, texh, chCount;
    uint8_t* pStonebricksPixels = stbi_load("Assets\\Textures\\Stonebricks.png", &texw, &texh, &chCount, 0);
    assert(pStonebricksPixels);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_StonebricksTexture);

    glTextureParameteri(m_StonebricksTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_StonebricksTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_StonebricksTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_StonebricksTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum internalFormat, format;
    
    if (chCount == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }
    else
    if (chCount == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }
    else
    {
        assert(1);
    }

    glTextureStorage2D(m_StonebricksTexture, 1, internalFormat, texw, texh);
    glTextureSubImage2D(m_StonebricksTexture, 0, 0, 0, texw, texh, format, GL_UNSIGNED_BYTE, pStonebricksPixels);

    stbi_image_free(pStonebricksPixels);

    return true;
}

void Application::MainLoop()
{
    MSG msg;
    memset(&msg, 0, sizeof(msg));

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER previousTickCount;
    QueryPerformanceCounter(&previousTickCount);

    while (msg.message != WM_QUIT)
    {
        LARGE_INTEGER currentTickCount;
        QueryPerformanceCounter(&currentTickCount);

        ULONGLONG elapsedTickCount = currentTickCount.QuadPart - previousTickCount.QuadPart;

        ULONGLONG elapsedMicroseconds = (elapsedTickCount * 1'000'000);
        elapsedMicroseconds /= frequency.QuadPart;

        previousTickCount = currentTickCount;

        float deltaTime = (elapsedMicroseconds * 0.001f) * 0.001f;

        if (deltaTime >= 0.05f)
        {
            deltaTime = 0.05f;
        }

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (m_bKeyStates[VK_ESCAPE])
            {
                PostQuitMessage(0);
            }

            const float cameraSpeed = 2.5f;

            const glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

            glm::vec2 deltaMousePos = (m_CurrentMousePos - m_PrevMousePos) * 0.1f;
            m_PrevMousePos = m_CurrentMousePos;

            //if (m_bCameraMoving)
            //{
                if (m_bKeyStates[0x57]) //  W key
                {
                    m_CameraPosition += m_CameraForwardDir * cameraSpeed * deltaTime;
                }
                else
                if (m_bKeyStates[0x53]) // 	S key
                {
                    m_CameraPosition -= m_CameraForwardDir * cameraSpeed * deltaTime;
                }

                const glm::vec3 rightDirection = glm::cross(m_CameraForwardDir, upDirection);

                if (m_bKeyStates[0x41]) //  A key
                {
                    m_CameraPosition -= rightDirection * cameraSpeed * deltaTime;
                }
                else
                if (m_bKeyStates[0x44]) //  D key
                {
                    m_CameraPosition += rightDirection * cameraSpeed * deltaTime;
                }

                if (m_bKeyStates[0x51]) //     Q key
                {
                    m_CameraPosition += upDirection * cameraSpeed * deltaTime;
                }
                else
                if (m_bKeyStates[0x45])  //  E key
                {
                    m_CameraPosition -= upDirection * cameraSpeed * deltaTime;
                }

                //  sin(yaw) z     +            sin(pitch) y     +  
                //           |   / |                       |   / |
                //           | /   |                       | /   |
                //           ++ -  + x                     ++ -  + x/z
                //                  cos(yaw)                      cos(pitch)

                if (m_bCameraMoving && !(deltaMousePos.x == 0.0f && deltaMousePos.y == 0.0f))
                {
                    const float rotationSpeed = 0.55f;
                    m_Yaw += deltaMousePos.x * rotationSpeed;
                    m_Pitch += (-deltaMousePos.y) * rotationSpeed;

                    glm::vec3 newForwardDirection;
                    newForwardDirection.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                    newForwardDirection.y = sin(glm::radians(m_Pitch));
                    newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));

                    m_CameraForwardDir = glm::normalize(newForwardDirection);
                }
            //}

            glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
            glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(m_Program);

            m_View = glm::lookAt(m_CameraPosition, (m_CameraPosition + m_CameraForwardDir), upDirection);
            m_Projection = glm::perspective(glm::radians(45.0f), (m_ScreenWidth / static_cast<float>(m_ScreenHeight)), 0.001f, 1000.0f);

            glm::mat4 worldViewProjection = (m_Projection * m_View);
            glUniformMatrix4fv(glGetUniformLocation(m_Program, "u_WorldViewProjection"), 1, GL_FALSE, glm::value_ptr(worldViewProjection));

            glBindTextureUnit(0, m_StonebricksTexture);

            // TriangleA
            glm::mat4 world = glm::translate(glm::mat4(1.0f), glm::vec3(m_TriangleAPosition));
            glUniformMatrix4fv(glGetUniformLocation(m_Program, "u_World"), 1, GL_FALSE, glm::value_ptr(world));

            glBindVertexArray(m_VertexArray);
            glDrawArrays(GL_TRIANGLES, 0, (m_TriangleVertices.size() / 5));

            // RectangleA
            world = glm::translate(glm::mat4(1.0f), glm::vec3(m_RectangleAPosition));
            glUniformMatrix4fv(glGetUniformLocation(m_Program, "u_World"), 1, GL_FALSE, glm::value_ptr(world));

            glBindVertexArray(m_VertexArray);
            glDrawArrays(GL_TRIANGLES, (m_TriangleVertices.size() / 5), (m_RectangleVertices.size() / 5));

            wglSwapIntervalEXT(0);
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
            m_bKeyStates[wParam] = true;

            //if (wParam == VK_F1)
            //{
            //    ShowCursor(FALSE);
            //    
            //}
            //else
            //if (wParam == VK_F2)
            //{
            //    ShowCursor(TRUE);
            //}

            //printf("Key Down: %llu \n", wParam);

            return 0;
        };

        case WM_KEYUP:
        {
            m_bKeyStates[wParam] = false;
            return 0;
        };

        case WM_MOUSEMOVE:
        {
            POINT pos;

            if (GetCursorPos(&pos))
            {
                ScreenToClient(m_hWindow, &pos);
                m_CurrentMousePos = glm::vec2(pos.x, pos.y);
            }

            //printf("Current Mouse Pos: x{%g} y{%g} \n", m_CurrentMousePos.x, m_CurrentMousePos.y);

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            m_bCameraMoving = true;
            return 0;
        }

        case WM_RBUTTONUP:
        {
            m_bCameraMoving = false;
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(m_hWindow, uMsg, wParam, lParam);
}