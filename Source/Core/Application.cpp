#include "Application.h"

#include <cstdio>

const wchar_t ClassName[] = TEXT("MyWindow");

GLuint texture_Sky = 0;

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

void LoadTexture(StrongTexturePtr& texture, const std::string& file)
{
    int width, height, channelCount;
    uint8_t* pPixels = stbi_load(file.c_str(), &width, &height, &channelCount, 0);
    assert(pPixels);

    //m_Texture_Stonebricks.reset(new Texture());
    //assert(m_Texture_Stonebricks);
    //
    //m_Texture_Stonebricks->Create(GL_TEXTURE_2D);
    //
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    //m_Texture_Stonebricks->SetStorage2D(1, InternalFormat(channelCount), width, height);
    //m_Texture_Stonebricks->SetSubImage2D(0, 0, 0, width, height, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);

    texture.reset(new Texture());
    assert(texture);
    texture->Create(GL_TEXTURE_2D);
    
    texture->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    texture->SetStorage2D(1, InternalFormat(channelCount), width, height);
    texture->SetSubImage2D(0, 0, 0, width, height, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);

    stbi_image_free(pPixels);
}

void LoadShader(StrongShaderProgPtr& shaderProg, const std::string& vsFile, const std::string& fsFile)
{
    char* pVertShaderSource = _ReadFile(vsFile);
    char* pFragShaderSource = _ReadFile(fsFile);

    auto vertShader = std::make_shared<Shader>();
    assert(vertShader);
    vertShader->Create(GL_VERTEX_SHADER);
    vertShader->SetSource(1, &pVertShaderSource, nullptr);
    vertShader->Compile();

    auto fragShader = std::make_shared<Shader>();
    assert(fragShader);
    fragShader->Create(GL_FRAGMENT_SHADER);
    fragShader->SetSource(1, &pFragShaderSource, nullptr);
    fragShader->Compile();

    shaderProg.reset(new ShaderProgram());
    assert(shaderProg);
    shaderProg->Create();
    shaderProg->AttachShader(vertShader);
    shaderProg->AttachShader(fragShader);
    shaderProg->Link();

    delete[] pFragShaderSource;
    delete[] pVertShaderSource;
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
 #if 0
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
 #endif

    //  ShaderProgs
    LoadShader(m_ShaderProg_Textured, "Assets\\Shaders\\Textured_vert.glsl", "Assets\\Shaders\\Textured_frag.glsl");
    LoadShader(m_ShaderProg_Sky, "Assets\\Shaders\\Sky_vert.glsl", "Assets\\Shaders\\Sky_frag.glsl");

    //  Textures

    stbi_set_flip_vertically_on_load(true);

    //int width, height, channelCount;
    //uint8_t* pPixels = stbi_load("Assets\\Textures\\Stonebricks.png", &width, &height, &channelCount, 0);
    //assert(pPixels);

    //m_Texture_Stonebricks.reset(new Texture());
    //assert(m_Texture_Stonebricks);

    //m_Texture_Stonebricks->Create(GL_TEXTURE_2D);
    
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //m_Texture_Stonebricks->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //m_Texture_Stonebricks->SetStorage2D(1, InternalFormat(channelCount), width, height);
    //m_Texture_Stonebricks->SetSubImage2D(0, 0, 0, width, height, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);

    //stbi_image_free(pPixels);

    LoadTexture(m_Texture_Stonebricks, "Assets\\Textures\\Stonebricks.png");

    std::vector<std::string> skyTextureNames =
    {
        "Assets\\Textures\\Sky_Right.png",
        "Assets\\Textures\\Sky_Left.png",
        "Assets\\Textures\\Sky_Top.png",
        "Assets\\Textures\\Sky_Bottom.png",
        "Assets\\Textures\\Sky_Front.png",
        "Assets\\Textures\\Sky_Back.png",
    };

    stbi_set_flip_vertically_on_load(false);

 #define DSA_SKYBOX 1
 #if DSA_SKYBOX
    m_Texture_Sky.reset(new Texture());
    assert(m_Texture_Sky);
    m_Texture_Sky->Create(GL_TEXTURE_CUBE_MAP);

    m_Texture_Sky->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_Texture_Sky->SetParamateri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channelCount;
    stbi_load(skyTextureNames[0].c_str(), &width, &height, &channelCount, 0);

    m_Texture_Sky->SetStorage2D(1, InternalFormat(channelCount), width, height);

    for (uint32_t i = 0; i < skyTextureNames.size(); ++i)
    {
        uint8_t* pPixels = stbi_load(skyTextureNames[i].c_str(), &width, &height, &channelCount, 0);
        assert(pPixels);
    
        m_Texture_Sky->SetSubImage3D(0, 0, 0, i, width, height, 1, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);
    }
 #else
    glGenTextures(1, &texture_Sky);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_Sky);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (uint32_t i = 0; i < skyTextureNames.size(); i++)
    {
        int width, height, channelCount;
        uint8_t* pPixels = stbi_load(skyTextureNames[i].c_str(), &width, &height, &channelCount, 0);
        assert(pPixels);
    
        //m_Texture_Sky->SetSubImage3D(0, 0, 0, i, width, height, 1, Format(channelCount), GL_UNSIGNED_BYTE, pPixels);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InternalFormat(channelCount), 
            width, height, 0, Format(channelCount), GL_UNSIGNED_BYTE, pPixels
        );

        stbi_image_free(pPixels);
    }
 #endif

    // --- Scene ------------------------------------------

    std::vector<Vertex> cubeVertices = CreateCubeVertices();

    m_Mesh_Cube.reset(new Mesh());
    assert(m_Mesh_Cube);
    m_Mesh_Cube->VertexCount = cubeVertices.size();
    m_Mesh_Cube->pVertices = cubeVertices.data();
    m_Mesh_Cube->VertexBufferOffset = 0;
    m_Mesh_Cube->Create();

    cubeVertices.clear();

    m_SceneNodes.reserve(32);

    m_Camera.reset(new CameraNode());
    m_Camera->VCreate();
    m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    m_SceneNodes.push_back(m_Camera);

    //auto sceneNode1 = m_SceneNodes.emplace_back(new MeshNode(m_VertexArray, m_Mesh_Cube, nullptr));
    auto sceneNode1 = m_SceneNodes.emplace_back(new MeshNode(m_Mesh_Cube, m_ShaderProg_Textured, m_Texture_Stonebricks));
    sceneNode1->VCreate();
    sceneNode1->SetPosition(glm::vec3(-2.5f, 0.0f, 0.0f));

    //auto sceneNode2 = m_SceneNodes.emplace_back(new MeshNode(m_VertexArray_Multiple, m_Mesh_Rectangle, m_Texture_Stonebricks));
    auto sceneNode2 = m_SceneNodes.emplace_back(new MeshNode(m_Mesh_Cube, m_ShaderProg_Textured, m_Texture_Stonebricks));
    sceneNode2->VCreate();
    sceneNode2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    //auto sceneNode3 = m_SceneNodes.emplace_back(new MeshNode(m_VertexArray_Multiple, m_Mesh_Triangle, m_Texture_Stonebricks));
    auto sceneNode3 = m_SceneNodes.emplace_back(new MeshNode(m_Mesh_Cube, m_ShaderProg_Textured, m_Texture_Stonebricks));
    sceneNode3->VCreate();
    sceneNode3->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));

    // ----------------------------------------------------

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

        glm::vec2 deltaMousePos = (m_CurrentMousePos - m_PrevMousePos) * 0.2f;
        m_PrevMousePos = m_CurrentMousePos;

        //if (m_bCameraMoving)
        //{
            if (m_bKeyStates[0x57]) //  W key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (m_Camera->GetForwardDir() * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x53]) // 	S key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (m_Camera->GetForwardDir() * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }

            const glm::vec3 rightDirection = glm::cross(m_Camera->GetForwardDir(), upDirection);

            if (m_bKeyStates[0x41]) //  A key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (rightDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x44]) //  D key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (rightDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }

            if (m_bKeyStates[0x51]) //     Q key
            {
                glm::vec3 newPos = m_Camera->GetPosition() + (upDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
            }
            else
            if (m_bKeyStates[0x45])  //  E key
            {
                glm::vec3 newPos = m_Camera->GetPosition() - (upDirection * cameraSpeed * deltaTime);
                m_Camera->SetPosition(newPos);
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
                
                m_Camera->SetForwardDir(glm::normalize(newForwardDirection));
            }
        //}

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
        glClearColor(0.05f, 0.05f, 0.05f, 1.00f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        m_ShaderProg_Sky->Use();
        m_ShaderProg_Sky->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(m_Camera->GetView())));
        m_ShaderProg_Sky->SetUniformMatrix4f("u_WorldProjection", m_Camera->GetProjection());

     #if DSA_SKYBOX
        m_Texture_Sky->BindUnit(0);
     #else
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_Sky);
     #endif
        m_ShaderProg_Sky->SetUniform1i("u_Texture", 0);

        m_Mesh_Cube->m_VertexArray->Bind();
        glDrawArrays(GL_TRIANGLES, m_Mesh_Cube->VertexBufferOffset, m_Mesh_Cube->VertexCount);

        glDepthMask(GL_TRUE);

        m_ShaderProg_Textured->Use();
        m_ShaderProg_Textured->SetUniformMatrix4f("u_WorldViewProjection", m_Camera->WorldViewProjection());

        for (const auto& node : m_SceneNodes) 
        {
         #if 0
            node->VRender(m_ShaderProg_Textured);
         #endif

            node->VRender();
        }

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
            //else
            //if (wParam == VK_F1)
            //{
            //    m_bWireframeEnabled = !m_bWireframeEnabled;
            //}

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