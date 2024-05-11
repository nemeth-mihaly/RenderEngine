#include "Application.h"

#include <cstdio>

const wchar_t ClassName[] = TEXT("MyWindow");

char* _ReadFile(const std::string& name)
{
    FILE* fp = fopen(name.c_str(), "rb");
    assert(fp != nullptr);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* buf = new char[size + 1];
    size_t bytesRead = fread(buf, sizeof(char), size, fp);
    assert(bytesRead >= size);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}

// Window procedure wrapped in C++ class:
//      https://devblogs.microsoft.com/oldnewthing/20191014-00/?p=102992

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

////////////////////////////////////////////////////
//  Application Implementation
////////////////////////////////////////////////////

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
    m_ScreenWidth = 1280;
    m_ScreenHeight = 720;

    m_hWindow = nullptr;

    memset(m_bKeyStates, 0, sizeof(m_bKeyStates));

    m_hDeviceContext = nullptr;
    m_hGLRenderContext = nullptr;

    m_bIsRunning = false;

    m_CubeAPosition = glm::vec3(-5.0f, 0.0f, 0.0f);
    m_TriangleAPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    m_RectangleAPosition = glm::vec3(5.0f, 0.0f, 0.0f);

    m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    m_CameraForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);

    m_bCameraMoving = false;

    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    m_bWireframeEnabled = false;
}

Application::~Application()
{
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
        TEXT("TestApp"),
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

    // Vertex inputs

    std::vector<Vertex> triangleVertices = CreateTriangleVertices();
    std::vector<Vertex> rectangleVertices = CreateRectangleVertices();

    uint32_t vertexBufferSize_Textured = 0;

    m_Mesh_Triangle.reset(new Mesh());
    assert(m_Mesh_Triangle);
    m_Mesh_Triangle->VertexCount = triangleVertices.size();
    m_Mesh_Triangle->pVertices = triangleVertices.data();
    m_Mesh_Triangle->VertexBufferOffset = vertexBufferSize_Textured;

    vertexBufferSize_Textured += m_Mesh_Triangle->VertexCount;

    m_Mesh_Rectangle.reset(new Mesh());
    assert(m_Mesh_Rectangle);
    m_Mesh_Rectangle->VertexCount = rectangleVertices.size();
    m_Mesh_Rectangle->pVertices = rectangleVertices.data();
    m_Mesh_Rectangle->VertexBufferOffset = vertexBufferSize_Textured;

    vertexBufferSize_Textured += m_Mesh_Rectangle->VertexCount;

    m_VertexBuffer_Multiple.reset(new Buffer());
    assert(m_VertexBuffer_Multiple);
    m_VertexBuffer_Multiple->Create();
    m_VertexBuffer_Multiple->SetData(
        sizeof(Vertex) * vertexBufferSize_Textured,
        nullptr,
        GL_STATIC_DRAW
    );

    m_VertexBuffer_Multiple->SetSubData(
        sizeof(Vertex) * m_Mesh_Triangle->VertexBufferOffset,
        sizeof(Vertex) * m_Mesh_Triangle->VertexCount,
        m_Mesh_Triangle->pVertices
    );

    m_VertexBuffer_Multiple->SetSubData(
        sizeof(Vertex) * m_Mesh_Rectangle->VertexBufferOffset, 
        sizeof(Vertex) * m_Mesh_Rectangle->VertexCount, 
        m_Mesh_Rectangle->pVertices
    );

    triangleVertices.clear();
    rectangleVertices.clear();

    m_VertexArray_Multiple.reset(new VertexArray());
    assert(m_VertexArray_Multiple);
    m_VertexArray_Multiple->Create();
    m_VertexArray_Multiple->SetVertexBuffer(m_VertexBuffer_Multiple, sizeof(Vertex));
    m_VertexArray_Multiple->SetVertexInputAttribute(0, 3, GL_FLOAT, 0);
    m_VertexArray_Multiple->SetVertexInputAttribute(1, 3, GL_FLOAT, 12);
    m_VertexArray_Multiple->SetVertexInputAttribute(2, 2, GL_FLOAT, 24);

    std::vector<Vertex> cubeVertices = CreateCubeVertices();

    uint32_t vertexBufferSize_Position = 0;

    m_Mesh_Cube.reset(new Mesh());
    assert(m_Mesh_Cube);
    m_Mesh_Cube->VertexCount = cubeVertices.size();
    m_Mesh_Cube->pVertices = cubeVertices.data();
    m_Mesh_Cube->VertexBufferOffset = vertexBufferSize_Position;

    vertexBufferSize_Position += m_Mesh_Cube->VertexCount;

    m_VertexBuffer.reset(new Buffer());
    assert(m_VertexBuffer);
    m_VertexBuffer->Create();
    m_VertexBuffer->SetData(
        sizeof(Vertex) * vertexBufferSize_Position, 
        m_Mesh_Cube->pVertices, 
        GL_STATIC_DRAW
    );

    cubeVertices.clear();

    m_VertexArray.reset(new VertexArray());
    assert(m_VertexArray);
    m_VertexArray->Create();
    m_VertexArray->SetVertexBuffer(m_VertexBuffer, sizeof(Vertex));
    m_VertexArray->SetVertexInputAttribute(0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexInputAttribute(1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexInputAttribute(2, 2, GL_FLOAT, 24);

    //  ShaderProgs

    char* pVertShaderSource = _ReadFile("Assets\\Shaders\\Textured_vert.glsl");
    char* pFragShaderSource = _ReadFile("Assets\\Shaders\\Textured_frag.glsl");

    auto vertShader_Textured = std::make_shared<Shader>();
    assert(vertShader_Textured);
    vertShader_Textured->Create(GL_VERTEX_SHADER);
    vertShader_Textured->SetSource(1, &pVertShaderSource, nullptr);
    vertShader_Textured->Compile();

    auto fragShader_Textured = std::make_shared<Shader>();
    assert(fragShader_Textured);
    fragShader_Textured->Create(GL_FRAGMENT_SHADER);
    fragShader_Textured->SetSource(1, &pFragShaderSource, nullptr);
    fragShader_Textured->Compile();

    m_ShaderProg_Textured.reset(new ShaderProgram());
    assert(m_ShaderProg_Textured);
    m_ShaderProg_Textured->Create();
    m_ShaderProg_Textured->AttachShader(vertShader_Textured);
    m_ShaderProg_Textured->AttachShader(fragShader_Textured);
    m_ShaderProg_Textured->Link();

    delete[] pFragShaderSource;
    delete[] pVertShaderSource;

    //  Textures

    stbi_set_flip_vertically_on_load(true);

    int width, height, channelCount;
    uint8_t* pPixels = stbi_load("Assets\\Textures\\Stonebricks.png", &width, &height, &channelCount, 0);
    assert(pPixels);

    m_Texture_Stonebricks.reset(new Texture());
    assert(m_Texture_Stonebricks);

    m_Texture_Stonebricks->Create(GL_TEXTURE_2D);
    
    m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_Texture_Stonebricks->SetStorage2D(1, InternalFormat(channelCount), width, height);
    m_Texture_Stonebricks->SetSubImage2D(0, 0, 0, width, height, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);

    stbi_image_free(pPixels);

    m_SceneNode1.reset(new SceneNode());
    m_SceneNode1->SetPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
    
    m_SceneNode2.reset(new SceneNode());
    m_SceneNode2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    m_SceneNode3.reset(new SceneNode());
    m_SceneNode3->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));

    m_bIsRunning = true;
    return true;
}

void Application::MainLoop()
{
    MSG msg;
    memset(&msg, 0, sizeof(msg));

    //LARGE_INTEGER frequency;
    //QueryPerformanceFrequency(&frequency);

    //LARGE_INTEGER previousTickCount;
    //QueryPerformanceCounter(&previousTickCount);

    std::chrono::_V2::system_clock::time_point previous;
    previous = std::chrono::high_resolution_clock::now();

    while (m_bIsRunning)
    {
        //LARGE_INTEGER currentTickCount;
        //QueryPerformanceCounter(&currentTickCount);

        //ULONGLONG elapsedTickCount = currentTickCount.QuadPart - previousTickCount.QuadPart;

        //ULONGLONG elapsedMicroseconds = (elapsedTickCount * 1'000'000);
        //elapsedMicroseconds /= frequency.QuadPart;

        //previousTickCount = currentTickCount;

        //float deltaTime = (elapsedMicroseconds * 0.001f) * 0.001f;

        std::chrono::_V2::system_clock::time_point current;
        current = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration<float>(current - previous).count();
        previous = current;

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                m_bIsRunning = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
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

            /*
            //  sin(yaw) z     +            sin(pitch) y     +  
            //           |   / |                       |   / |
            //           | /   |                       | /   |
            //           ++ -  + x                     ++ -  + x/z
            //              cos(yaw)                        cos(pitch)
            */    

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

        m_View = glm::lookAt(m_CameraPosition, (m_CameraPosition + m_CameraForwardDir), upDirection);
        m_Projection = glm::perspective(glm::radians(45.0f), (m_ScreenWidth / static_cast<float>(m_ScreenHeight)), 0.001f, 1000.0f);

        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
        glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ShaderProg_Textured->Use();
        glm::mat4 worldViewProjection = (m_Projection * m_View);
        m_ShaderProg_Textured->SetUniformMatrix4f("u_WorldViewProjection", worldViewProjection);

        // CubeA
        glm::mat4 world = glm::translate(glm::mat4(1.0f), m_SceneNode1->GetPosition());
        m_ShaderProg_Textured->SetUniformMatrix4f("u_World", world);
        m_ShaderProg_Textured->SetUniform1b("u_bHasTexture", false);

        if (m_bWireframeEnabled)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        m_VertexArray->Bind();
        glDrawArrays(GL_TRIANGLES, m_Mesh_Cube->VertexBufferOffset, m_Mesh_Cube->VertexCount);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // TriangleA
        m_Texture_Stonebricks->BindUnit(0);
        m_ShaderProg_Textured->SetUniform1ui("u_Texture", 0);
        m_ShaderProg_Textured->SetUniform1b("u_bHasTexture", false);

        world = glm::translate(glm::mat4(1.0f), m_SceneNode2->GetPosition());
        m_ShaderProg_Textured->SetUniformMatrix4f("u_World", world);

        m_VertexArray_Multiple->Bind();
        glDrawArrays(GL_TRIANGLES, m_Mesh_Triangle->VertexBufferOffset, m_Mesh_Triangle->VertexCount);

        // RectangleA
        m_ShaderProg_Textured->SetUniform1b("u_bHasTexture", true);
        world = glm::translate(glm::mat4(1.0f), m_SceneNode3->GetPosition());
        m_ShaderProg_Textured->SetUniformMatrix4f("u_World", world);

        glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);

        wglSwapIntervalEXT(0);
        wglSwapLayerBuffers(m_hDeviceContext, WGL_SWAP_MAIN_PLANE);
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
            else
            if (wParam == VK_F1)
            {
                m_bWireframeEnabled = !m_bWireframeEnabled;
            }

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

        case WM_CLOSE:
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(m_hWindow, uMsg, wParam, lParam);
}