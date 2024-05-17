#include "Application.h"

#include <cstring>
#include <format>
#include <iostream>

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

void LoadWavefrontModel(std::vector<Vertex>& vertices, const std::string& file)
{
    FILE* fp = fopen(file.c_str(), "r");
    assert(fp != nullptr);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    while (true)
    {
        char line[512];
        int result = fscanf(fp, "%s", line);
        
        if (result == EOF)
        {
            break;
        }

        if (strcmp(line, "v") == 0)
        {
            glm::vec3 pos;
            fscanf(fp, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            positions.push_back(pos);
        }
        else
        if (strcmp(line, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }
        else
        if (strcmp(line, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        }
        else
        if (strcmp(line, "f") == 0)
        {
            // [0]: v; [1]: vt; [2]: vn;
            uint32_t faceIndices1[3];
            uint32_t faceIndices2[3];
            uint32_t faceIndices3[3];

            fscanf(
                fp, 
                "%u/%u/%u %u/%u/%u %u/%u/%u\n", 
                &faceIndices1[0], &faceIndices1[1], &faceIndices1[2],
                &faceIndices2[0], &faceIndices2[1], &faceIndices2[2], 
                &faceIndices3[0], &faceIndices3[1], &faceIndices3[2]
            );

            auto GetVertexFromFace = [&positions, &normals, &uvs](uint32_t indices[3])
            {
                Vertex vertex;

                vertex.Pos = positions[(indices[0] - 1)];
                vertex.Normal = normals[(indices[2] - 1)];
                vertex.Uv = uvs[(indices[1] - 1)];  

                return vertex;
            };

            vertices.push_back(GetVertexFromFace(faceIndices1));
            vertices.push_back(GetVertexFromFace(faceIndices2));
            vertices.push_back(GetVertexFromFace(faceIndices3));
        }
    }

    fclose(fp);
}

void LoadTexture(StrongTexturePtr& texture, const std::string& file)
{
    int width, height, channelCount;
    uint8_t* pPixels = stbi_load(file.c_str(), &width, &height, &channelCount, 0);
    assert(pPixels);

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
    m_ScreenWidth = (wr.right - wr.left);
    m_ScreenHeight = (wr.bottom - wr.top);

    m_hWindow = CreateWindowEx(
        0,
        ClassName,
        TEXT("Render Engine | Testbed"),
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
        0, // End.
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

    LoadShader(m_ShaderProg_TexturedLit, "Assets\\Shaders\\TexturedLit_vert.glsl", "Assets\\Shaders\\TexturedLit_frag.glsl");
    LoadShader(m_ShaderProg_Sky, "Assets\\Shaders\\Sky_vert.glsl", "Assets\\Shaders\\Sky_frag.glsl");
    LoadShader(m_ShaderProg_FramebufferTest, "Assets\\Shaders\\Framebuffer_test_vert.glsl", "Assets\\Shaders\\Framebuffer_test_frag.glsl");

    stbi_set_flip_vertically_on_load(true);
    LoadTexture(m_Texture_Stonebricks, "Assets\\Textures\\Stonebricks.png");
    LoadTexture(m_Texture_Grass, "Assets\\Textures\\Grass.png");

    std::vector<std::string> skyTextureNames =
    {
        "Assets\\Textures\\Sky_PX.png",
        "Assets\\Textures\\Sky_NX.png",
        "Assets\\Textures\\Sky_PY.png",
        "Assets\\Textures\\Sky_NY.png",
        "Assets\\Textures\\Sky_PZ.png",
        "Assets\\Textures\\Sky_NZ.png",
    };

    stbi_set_flip_vertically_on_load(false);

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

    std::vector<Vertex> rectangleVertices = CreateRectangleVertices();

    m_Mesh_Rectangle.reset(new Mesh());
    assert(m_Mesh_Rectangle);
    m_Mesh_Rectangle->VertexCount = rectangleVertices.size();
    m_Mesh_Rectangle->pVertices = rectangleVertices.data();
    m_Mesh_Rectangle->VertexBufferOffset = 0;
    m_Mesh_Rectangle->Create();
    rectangleVertices.clear();

    std::vector<Vertex> cubeVertices = CreateCubeVertices();

    m_Mesh_Cube.reset(new Mesh());
    assert(m_Mesh_Cube);
    m_Mesh_Cube->VertexCount = cubeVertices.size();
    m_Mesh_Cube->pVertices = cubeVertices.data();
    m_Mesh_Cube->VertexBufferOffset = 0;
    m_Mesh_Cube->Create();
    cubeVertices.clear();

    std::vector<Vertex> wavefrontCubeVertices;
    LoadWavefrontModel(wavefrontCubeVertices, "Assets\\Models\\Cube.obj");

    m_WavefrontMesh_Cube.reset(new Mesh());
    assert(m_WavefrontMesh_Cube);
    m_WavefrontMesh_Cube->VertexCount = wavefrontCubeVertices.size();
    m_WavefrontMesh_Cube->pVertices = wavefrontCubeVertices.data();
    m_WavefrontMesh_Cube->VertexBufferOffset = 0;
    m_WavefrontMesh_Cube->Create();
    wavefrontCubeVertices.clear();

    std::vector<Vertex> wavefrontMonkeyVertices;
    LoadWavefrontModel(wavefrontMonkeyVertices, "Assets\\Models\\Monkey.obj");

    m_WavefrontMesh_Monkey.reset(new Mesh());
    assert(m_WavefrontMesh_Monkey);
    m_WavefrontMesh_Monkey->VertexCount = wavefrontMonkeyVertices.size();
    m_WavefrontMesh_Monkey->pVertices = wavefrontMonkeyVertices.data();
    m_WavefrontMesh_Monkey->VertexBufferOffset = 0;
    m_WavefrontMesh_Monkey->Create();
    wavefrontMonkeyVertices.clear();

    m_SceneNodes.reserve(32);
    m_LightNodes.reserve(32);

    m_Camera.reset(new CameraNode());
    m_Camera->VCreate();
    m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_SceneNodes.push_back(m_Camera);

    auto floorNode = m_SceneNodes.emplace_back(new MeshNode(m_WavefrontMesh_Cube, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    floorNode->VCreate();
    floorNode->SetPosition(glm::vec3(0.0f, -0.55f, -0.0f));
    floorNode->SetScale(glm::vec3(10.0f, 0.1f, 10.0f));
    floorNode->GetMaterial().Diffuse = glm::vec3(0.22f, 0.28f, 0.13f);
    floorNode->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    floorNode->GetMaterial().bUseTexture = false;

    auto node1 = m_SceneNodes.emplace_back(new MeshNode(m_WavefrontMesh_Cube, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    node1->VCreate();
    node1->SetPosition(glm::vec3(-1.5f, 0.0f, -2.5f));
    node1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node1->GetMaterial().Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    node1->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    node1->GetMaterial().bUseTexture = true;

    auto node2 = m_SceneNodes.emplace_back(new MeshNode(m_WavefrontMesh_Cube, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    node2->VCreate();
    node2->SetPosition(glm::vec3(2.0f, 0.0f, -2.5f));
    node2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node2->GetMaterial().Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    node2->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    node2->GetMaterial().bUseTexture = true;

    auto node3 = m_SceneNodes.emplace_back(new MeshNode(m_WavefrontMesh_Cube, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    node3->VCreate();
    node3->SetPosition(glm::vec3(1.0f, 0.0f, -3.5f));
    node3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    node3->GetMaterial().Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    node3->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    node3->GetMaterial().bUseTexture = true;

    auto monkeyNode = m_SceneNodes.emplace_back(new MeshNode(m_WavefrontMesh_Monkey, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    monkeyNode->VCreate();
    monkeyNode->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    monkeyNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    monkeyNode->GetMaterial().Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    monkeyNode->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    monkeyNode->GetMaterial().bUseTexture = false;

    // DirectionalLightNode
    LightProperties directionalLightProperties;
    directionalLightProperties.Type = LightType::Directional;
    directionalLightProperties.Direction = glm::vec3(1.2f, -1.0f, 1.3f);
    directionalLightProperties.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLightProperties.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLightProperties.Specular = glm::vec3(1.0f, 1.0f, 1.0f);;

    m_DirectionalLightNode.reset(new LightNode());
    m_DirectionalLightNode->VCreate();
    m_DirectionalLightNode->SetLightProperties(directionalLightProperties);
    m_SceneNodes.push_back(m_DirectionalLightNode);
    m_LightNodes.push_back(m_DirectionalLightNode);

    // PointLightNode
    LightProperties pointLightProperties;
    pointLightProperties.Type = LightType::Point;
    pointLightProperties.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    pointLightProperties.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLightProperties.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLightProperties.Specular = glm::vec3(1.0f, 1.0f, 1.0f);;
    pointLightProperties.ConstantAttenuation = 1.0f;
    pointLightProperties.LinearAttenuation = 0.09f;
    pointLightProperties.QuadraticAttenuation = 0.032f;

    m_PointLightNode.reset(new LightNode());
    m_PointLightNode->VCreate();
    m_PointLightNode->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));
    m_PointLightNode->SetLightProperties(pointLightProperties);
    m_SceneNodes.push_back(m_PointLightNode);
    m_LightNodes.push_back(m_PointLightNode);

    // SpotLightNode
    LightProperties spotLightProperties;
    spotLightProperties.Type = LightType::Spot;
    spotLightProperties.Position = glm::vec3(3.4f, 1.0f, 3.0f);
    spotLightProperties.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spotLightProperties.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLightProperties.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLightProperties.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLightProperties.Falloff = cosf(glm::radians(30.0f));
    spotLightProperties.Phi = cosf(glm::radians(36.0f));
    spotLightProperties.ConstantAttenuation = 1.0f;
    spotLightProperties.LinearAttenuation = 0.09f;
    spotLightProperties.QuadraticAttenuation = 0.032f;

    m_SpotLightNode.reset(new LightNode());
    m_SpotLightNode->VCreate();
    m_SpotLightNode->SetPosition(glm::vec3(0.4f, 1.0f, -2.0f));
    m_SpotLightNode->SetLightProperties(spotLightProperties);
    m_SceneNodes.push_back(m_SpotLightNode);
    m_LightNodes.push_back(m_SpotLightNode);

    auto lightBulb = m_SceneNodes.emplace_back(new MeshNode(m_Mesh_Cube, m_ShaderProg_TexturedLit, m_Texture_Stonebricks));
    lightBulb->VCreate();
    lightBulb->SetPosition(pointLightProperties.Position);
    lightBulb->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    lightBulb->GetMaterial().Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    lightBulb->GetMaterial().Specular = glm::vec3(0.0f, 0.0f, 0.0f);
    lightBulb->GetMaterial().Emissive = glm::vec3(1.0f, 1.0f, 1.0f);
    lightBulb->GetMaterial().bUseTexture = false;

    // ----------------------------------------------------

    glGenFramebuffers(1, &m_FramebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

    glGenTextures(1, &m_TextureID_ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ScreenWidth, m_ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID_ColorBuffer, 0);

    glGenRenderbuffers(1, &m_RenderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, m_ScreenWidth, m_ScreenHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
                newForwardDirection.y = sinf(glm::radians(m_Pitch));
                newForwardDirection.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
                
                m_Camera->SetForwardDir(glm::normalize(newForwardDirection));
            }
        //}

        for (const auto& node : m_SceneNodes)
        {
            node->VUpdate(deltaTime);
        }


        glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
        glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Geometry phase

        m_ShaderProg_TexturedLit->Use();
        m_ShaderProg_TexturedLit->SetUniformMatrix4f("u_WorldViewProjection", m_Camera->WorldViewProjection());
        m_ShaderProg_TexturedLit->SetUniform3f("u_ViewPos", m_Camera->GetPosition());

        for (uint32_t i = 0; i < m_LightNodes.size(); ++i)
        {
            const std::string index = std::to_string(i);
            const LightProperties& lp = m_LightNodes[i]->GetLightProperties();

            m_ShaderProg_TexturedLit->SetUniform1i(("u_Lights[" + index + "].Type"), static_cast<int>(lp.Type));
            m_ShaderProg_TexturedLit->SetUniform3f(("u_Lights[" + index + "].Position"), lp.Position);
            m_ShaderProg_TexturedLit->SetUniform3f(("u_Lights[" + index + "].Direction"), lp.Direction);
            m_ShaderProg_TexturedLit->SetUniform3f(("u_Lights[" + index + "].Ambient"), lp.Ambient);
            m_ShaderProg_TexturedLit->SetUniform3f(("u_Lights[" + index + "].Diffuse"), lp.Diffuse);
            m_ShaderProg_TexturedLit->SetUniform3f(("u_Lights[" + index + "].Specular"), lp.Specular);
            m_ShaderProg_TexturedLit->SetUniform1f(("u_Lights[" + index + "].Falloff"), lp.Falloff);
            m_ShaderProg_TexturedLit->SetUniform1f(("u_Lights[" + index + "].ConstantAttenuation"), lp.ConstantAttenuation);
            m_ShaderProg_TexturedLit->SetUniform1f(("u_Lights[" + index + "].LinearAttenuation"), lp.LinearAttenuation);
            m_ShaderProg_TexturedLit->SetUniform1f(("u_Lights[" + index + "].QuadraticAttenuation"), lp.QuadraticAttenuation);
        }

        for (const auto& node : m_SceneNodes) 
        {
         #if 0
            node->VRender(m_ShaderProg_Textured);
         #endif

            node->VRender();
        }

     #if 1
        std::vector<glm::vec3> grassPositions =
        {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3( 1.5f, 0.0f, 0.51f),
            glm::vec3( 0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
        };

        for (uint32_t i = 0; i < grassPositions.size(); ++i)
        {
            m_ShaderProg_TexturedLit->SetUniform3f("u_Material.Ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_ShaderProg_TexturedLit->SetUniform3f("u_Material.Diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
            m_ShaderProg_TexturedLit->SetUniform3f("u_Material.Specular", glm::vec3(0.0f, 0.0f, 0.0f));
            m_ShaderProg_TexturedLit->SetUniform3f("u_Material.Emissive", glm::vec3(0.0f, 0.0f, 0.0f));
            m_ShaderProg_TexturedLit->SetUniform1f("u_Material.Power", 32.0f);
            m_ShaderProg_TexturedLit->SetUniform1b("u_Material.bUseTexture", true);
            
            if (true) // Normally this would be: if(Material.bUseTexture) {}
            {
                m_Texture_Grass->BindUnit(0);
                m_ShaderProg_TexturedLit->SetUniform1i("u_Texture", 0);
            }

            m_Mesh_Rectangle->m_VertexArray->Bind();

            glm::mat4 world = glm::translate(glm::mat4(1.0f), grassPositions[i]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
            m_ShaderProg_TexturedLit->SetUniformMatrix4f("u_World", world);
            glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);

            world = glm::translate(glm::mat4(1.0f), grassPositions[i]) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
            * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
            m_ShaderProg_TexturedLit->SetUniformMatrix4f("u_World", world);

            glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);
        }       
     #endif

        // Sky phase

        glDepthFunc(GL_LEQUAL);
        m_ShaderProg_Sky->Use();
        m_ShaderProg_Sky->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(m_Camera->GetView())));
        m_ShaderProg_Sky->SetUniformMatrix4f("u_WorldProjection", m_Camera->GetProjection());
        m_Texture_Sky->BindUnit(0);
        m_ShaderProg_Sky->SetUniform1i("u_Texture", 0);
        m_Mesh_Cube->m_VertexArray->Bind();
        glDrawArrays(GL_TRIANGLES, m_Mesh_Cube->VertexBufferOffset, m_Mesh_Cube->VertexCount);
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_ShaderProg_FramebufferTest->Use();
        m_Mesh_Rectangle->m_VertexArray->Bind();
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, m_TextureID_ColorBuffer);
        glActiveTexture(GL_TEXTURE0);
        m_ShaderProg_FramebufferTest->SetUniform1i("u_Texture", 0);
        glDrawArrays(GL_TRIANGLES, m_Mesh_Rectangle->VertexBufferOffset, m_Mesh_Rectangle->VertexCount);

        wglSwapIntervalEXT(1);
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