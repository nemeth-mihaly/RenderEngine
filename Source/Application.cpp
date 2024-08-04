#include "Application.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_glfw.h"
#include "3rdParty/ImGui/imgui_impl_opengl3.h"

Application* g_pApp = nullptr;

//-----------------------------------------------------------------------------
// enum SceneNodeEditorType
//-----------------------------------------------------------------------------

enum SceneNodeEditorType
{
    SceneNodeEditorType_Integer,
    SceneNodeEditorType_Float,
    SceneNodeEditorType_Vec3,
    SceneNodeEditorType_String,
    SceneNodeEditorType_RGB,
    SceneNodeEditorType_RGBA,
};

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

static void CloseCallback(GLFWwindow* pWindow)
{
    g_pApp->OnClose();
}

static void ResizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_pApp->OnResize(width, height);
}

static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        g_pApp->OnKeyDown(key);
    }
    else if (action == GLFW_RELEASE)
    {
        g_pApp->OnKeyUp(key);
    }
}

static void MouseMoveCallback(GLFWwindow* pWindow, double xpos, double ypos)
{
    g_pApp->OnMouseMove(static_cast<int>(xpos), static_cast<int>(ypos));
}

static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        g_pApp->OnMouseButtonDown(button);
    }
    else if (action == GLFW_RELEASE)
    {
        g_pApp->OnMouseButtonUp(button);
    } 
}

Application::Application()
{
    g_pApp = this;

    m_bRunning = false;
    m_bWindowedMode = true;
    m_bMinimized = false;

    m_windowPos = glm::ivec2(0, 0);
    m_windowSize = glm::ivec2(0, 0);
    m_pWindow = nullptr;

    m_currentMousePos = glm::ivec2(0, 0);

    m_deltaTime = 0.0f;

    m_pRenderer = nullptr;

    m_pCameraController = nullptr;

    m_bAddChildSelectedNodeRequested = false;
    m_bDeleteSelectedNodeRequested = false;
    m_bGrabSelectedSceneNode = false;
    m_selectedSceneNode = nullptr;
}

Application::~Application()
{
    SaveScene("Assets/Scenes/TestScene.json");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_pRenderer)
    {
        delete m_pRenderer;
    }

    if (m_pWindow)
    {
        glfwDestroyWindow(m_pWindow);
    }

    glfwTerminate();

    if (g_pApp)
    {
        g_pApp = nullptr;
    }
}

bool Application::Init(int width, int height)
{
    glfwInit();

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    m_pWindow = glfwCreateWindow(width, height, "RenderEngine | Testbed", nullptr, nullptr);

    // Save the window position and size.
    glfwGetWindowPos(m_pWindow, &m_windowPos.x, &m_windowPos.y);
    glfwGetWindowSize(m_pWindow, &m_windowSize.x, &m_windowSize.y);

    glfwSetWindowCloseCallback(m_pWindow, CloseCallback);
    glfwSetWindowSizeCallback(m_pWindow, ResizeCallback);

    glfwSetKeyCallback(m_pWindow, KeyCallback);

    glfwSetCursorPosCallback(m_pWindow, MouseMoveCallback);
    glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);

    glfwMakeContextCurrent(m_pWindow);
    gladLoadGL();

    glfwSwapInterval(0);

    m_pRenderer = new Renderer();
    m_pRenderer->Init();

    m_world = std::make_shared<World>();
    m_world->Init();

    m_pRenderer->SetWorld(m_world);

    m_camera = std::make_shared<Camera>();
    m_camera->Init();
    m_camera->m_bDebugMode = true;

    m_pRenderer->SetCamera(m_camera);

    m_camera2 = std::make_shared<Camera>();
    m_camera2->Init();

    m_cameraController.Init(m_camera);
    m_pCameraController = &m_cameraController;

    m_cameraController2.Init(m_camera2);

    m_sceneNodeFactory.Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Setup SceneNode Editor
    std::fstream file("Assets/Editor/SceneNodeDefs.json");
    Json sceneNodeEditorSceneNodeDefsData = Json::parse(file);
    file.close();

    for (Json::iterator it = sceneNodeEditorSceneNodeDefsData["sceneNodeList"].begin(); it != sceneNodeEditorSceneNodeDefsData["sceneNodeList"].end(); it++)
    {
        std::string sceneNodeDefName = (*it)["name"].get<std::string>();
        m_sceneNodeEditorSceneNodeDefs.insert(std::make_pair(sceneNodeDefName, (*it)));
    }

    m_sceneNodeEditorTypemap.insert(std::make_pair("Integer", SceneNodeEditorType_Integer));
    m_sceneNodeEditorTypemap.insert(std::make_pair("Float", SceneNodeEditorType_Float));
    m_sceneNodeEditorTypemap.insert(std::make_pair("Vec3", SceneNodeEditorType_Vec3));
    m_sceneNodeEditorTypemap.insert(std::make_pair("String", SceneNodeEditorType_String));
    m_sceneNodeEditorTypemap.insert(std::make_pair("RGB", SceneNodeEditorType_RGB));
    m_sceneNodeEditorTypemap.insert(std::make_pair("RGBA", SceneNodeEditorType_RGBA));

    // ..
    LoadScene("Assets/Scenes/TestScene.json");

    // End initialization.
    m_bRunning = true;
    
    return true;
}

void Application::RunLoop()
{
    float lastTime = static_cast<float>(glfwGetTime());

    while (m_bRunning)
    {
        glfwPollEvents();

        if (!m_bMinimized)
        {
            double currentTime = static_cast<float>(glfwGetTime());;
            m_deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            m_pCameraController->Update(m_deltaTime);
            m_pRenderer->Update(m_deltaTime);

            for (RayHit hit : m_raycasts)
            {
                m_pRenderer->AddLine(hit.ray.origin, hit.ray.origin + hit.ray.direction * hit.t, glm::vec3(1, 1, 1));

                glm::vec3 p = hit.ray.origin + hit.ray.direction * hit.t;
                m_pRenderer->AddBox(p, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1, 0, 0));
                // m_pRenderer->AddBox(glm::vec3(glm::round(p.x), p.y, glm::round(p.z)), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1, 1, 1));
            }

            if (m_bGrabSelectedSceneNode && m_selectedSceneNode)
            {
                m_selectedSceneNode->SetPosition(m_mouseWorldPos);
            }

            m_brushPos = m_mouseWorldPos;
            m_brushPos.y += 0.05f;

            m_brushRadius = 2.0f;

            // m_pRenderer->AddCross(m_brushPos, 0.2f, glm::vec3(1, 1, 1));
            // m_pRenderer->AddCircle(m_brushPos, glm::vec3(0, 1, 0), m_brushRadius, glm::vec3(1, 1, 1));

            if (m_bLeftMouseButtonDown)
            {
                // std::vector<Vertex_UnlitTexturedColored*> verts = m_world->GetVertsSelectedByBrush(m_brushPos, m_brushRadius);
                
                // float change = 2.0f;
                // change *= m_deltaTime;

                // for (auto& vert : verts)
                // {
                //     vert->pos.y += change;
                // }

                // m_world->m_bStagingVertBufferChanged = true;

                
            }

            // Render
            m_camera->Render();
            m_camera2->Render();
            m_pRenderer->Render();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            if (ImGui::Begin("Debug Drawing"))
            {
                #define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

                int numVerts = m_pRenderer->GetLineRenderer()->GetNumVerts();
                int maxVerts = m_pRenderer->GetLineRenderer()->GetMaxVerts();

                float progress = (float)numVerts / (float)maxVerts;
                float fraction = IM_CLAMP(progress, 0.0f, 1.0f);

                char buf[32];
                sprintf(buf, "%i/%i", numVerts, maxVerts);
                
                ImGui::ProgressBar(fraction, ImVec2(0.0f, 0.0f), buf);

                #undef IM_CLAMP
            }

            ImGui::End();

            ShowSceneNodeTree();
            ShowSceneNodeEditor();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_pWindow);
        }
    }
}

void Application::ShowMouse()
{
    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Application::HideMouse()
{
    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

std::shared_ptr<Mesh> Application::GetMesh(const std::string& asset)
{
    std::shared_ptr<Mesh> mesh;

    std::unordered_map<std::string, std::shared_ptr<Mesh>>::iterator it = m_meshAssets.find(asset);
    
    if (it != m_meshAssets.end())
    {
        mesh = (*it).second;
    }
    else
    {
        mesh = std::make_shared<Mesh>();
        mesh->Load(asset);

        m_meshAssets[asset] = mesh;
    }

    return mesh;
}

std::shared_ptr<Texture> Application::GetTexture(const std::string& asset)
{
    std::shared_ptr<Texture> texture;

    std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator it = m_textureAssets.find(asset);
    
    if (it != m_textureAssets.end())
    {
        texture = (*it).second;
    }
    else
    {
        texture = std::make_shared<Texture>(GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR);
        texture->Load(asset);

        m_textureAssets[asset] = texture;
    }

    return texture; 
}

void Application::OnClose()
{
    m_bRunning = false;
}

void Application::OnResize(int width, int height)
{
    m_windowSize = glm::ivec2(width, height);

    m_bMinimized = (width == 0 || height == 0);

    m_pRenderer->OnResize(width, height);
}

void Application::OnKeyDown(int key)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            m_bRunning = false;

            break;
            
        case GLFW_KEY_R:
        {
            m_pRenderer->GetShader_UnlitTexturedColoredTerrain()->Load("Assets/Shaders/UnlitTexturedColoredTerrain.vert", "Assets/Shaders/UnlitTexturedColoredTerrain.frag");

            break;
        }

        case GLFW_KEY_G:
        {
            m_bGrabSelectedSceneNode = !m_bGrabSelectedSceneNode;

            break;
        }

        case GLFW_KEY_H:
        {
            m_world->SaveHeightmap();

            break;
        }

        case GLFW_KEY_F7:
        {
            m_pRenderer->ToggleDepthTestForDebugPass();

            break;
        }

        case GLFW_KEY_F8:
        {
            m_raycasts.clear();

            break;
        }

        case GLFW_KEY_F9:
        {
            m_pRenderer->SetCamera(m_camera);
            m_pCameraController->ResetInputStates();
            m_pCameraController = &m_cameraController;

            break;
        }

        case GLFW_KEY_F10:
        {
            m_pRenderer->SetCamera(m_camera2);
            m_pCameraController->ResetInputStates();
            m_pCameraController = &m_cameraController2;

            break;
        }

        case GLFW_KEY_F11:
        {
            if (!m_bWindowedMode)
            {
                glfwSetWindowMonitor(m_pWindow, nullptr, m_windowPos.x, m_windowPos.y, m_windowSize.x, m_windowSize.y, 0);
            }
            else
            {
                // Save the window position and size to restore the previous settings when switching back to windowed mode.
                glfwGetWindowPos(m_pWindow, &m_windowPos.x, &m_windowPos.y);
                glfwGetWindowSize(m_pWindow, &m_windowSize.x, &m_windowSize.y);

                GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

                const GLFWvidmode* pVideoMode = glfwGetVideoMode(pMonitor);

                glfwSetWindowMonitor(m_pWindow, pMonitor, 0, 0, pVideoMode->width, pVideoMode->height, 0);
            }

            m_bWindowedMode = !m_bWindowedMode;

            break;
        }

        default:
            break;
    }

    m_pCameraController->OnKeyDown(key);
}

void Application::OnKeyUp(int key)
{
    m_pCameraController->OnKeyUp(key);
}

void Application::OnMouseMove(int x, int y)
{
    m_currentMousePos = glm::ivec2(x, y);

    auto camera = m_pRenderer->GetCamera();

    Ray ray;
    ray.origin = camera->GetPosition();
    ray.direction = camera->ScreenPointToWorldDirection(GetMousePosition());

    // /*
    float t = 0.0f;
    if (m_world->Raycast(ray, t))
    {
        RayHit hit;
        hit.ray = ray;
        hit.t = t;

        m_mouseWorldPos = ray.origin + ray.direction * hit.t; 
    }
    // */

    m_pCameraController->OnMouseMove(x, y);
}

void Application::OnMouseButtonDown(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            m_bLeftMouseButtonDown = true;

            /*
            auto camera = m_pRenderer->GetCamera();
            
            Ray ray;
            ray.origin = camera->GetPosition();
            ray.direction = camera->ScreenPointToWorldDirection(GetMousePosition());

            float t;
            if (m_world->Raycast(ray, t))
            {
                RayHit hit;
                hit.ray = ray;
                hit.t   = t;

                m_raycasts.push_back(hit);
            }
            */

            break;
        }

        default:
            break;
    }

    m_pCameraController->OnMouseButtonDown(button);
}

void Application::OnMouseButtonUp(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            m_bLeftMouseButtonDown = false;

            break;
        }

        default:
            break;
    }

    m_pCameraController->OnMouseButtonUp(button);    
}

void Application::ShowSceneNodeTree()
{
    if (ImGui::Begin("SceneNode Tree"))
    {
        std::shared_ptr<SceneNode> root = m_pRenderer->GetSceneGraphRoot();
        ShowSceneNodeTreeNode(root);
    }

    ImGui::End();

    if (m_bAddChildSelectedNodeRequested)
    {
        if (ImGui::Begin("Create New Node", &m_bAddChildSelectedNodeRequested))
        {
            if (ImGui::Button("PawnNode"))
            {
                std::fstream file("Assets/SceneNodes/PawnNode.json");
                Json data = Json::parse(file);
                file.close();

                m_selectedSceneNode->AddChild(m_sceneNodeFactory.CreateSceneNode(data));

                m_bAddChildSelectedNodeRequested = false;
            }

            if (ImGui::Button("TestNode"))
            {
                std::fstream file("Assets/SceneNodes/TestNode.json");
                Json data = Json::parse(file);
                file.close();

                m_selectedSceneNode->AddChild(m_sceneNodeFactory.CreateSceneNode(data));

                m_bAddChildSelectedNodeRequested = false;
            }

            if (ImGui::Button("MeshNode"))
            {
                std::fstream file("Assets/SceneNodes/MeshNode.json");
                Json data = Json::parse(file);
                file.close();

                m_selectedSceneNode->AddChild(m_sceneNodeFactory.CreateSceneNode(data));

                m_bAddChildSelectedNodeRequested = false;
            }

            if (ImGui::Button("LightNode"))
            {
                std::fstream file("Assets/SceneNodes/LightNode.json");
                Json data = Json::parse(file);
                file.close();

                m_selectedSceneNode->AddChild(m_sceneNodeFactory.CreateSceneNode(data));

                m_bAddChildSelectedNodeRequested = false;
            }
        }
        
        ImGui::End();
    }

    if (m_bDeleteSelectedNodeRequested)
    {
        SceneNode* pSelectedSceneNodeParent = m_selectedSceneNode->GetParent();

        if (pSelectedSceneNodeParent)
        {
            pSelectedSceneNodeParent->RemoveChild(m_selectedSceneNode);
        }

        m_bDeleteSelectedNodeRequested = false;
        m_selectedSceneNode = nullptr;
    }
}

void Application::ShowSceneNodeTreeNode(std::shared_ptr<SceneNode> node)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    if (m_selectedSceneNode == node)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (node->GetChildren().empty()) 
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool bTreeNodeOpen = ImGui::TreeNodeEx(node->GetName().c_str(), flags);

    if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
    {
        m_selectedSceneNode = node;
    }

    if (m_selectedSceneNode == node && ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Add Child Node...")) 
        {
            m_bAddChildSelectedNodeRequested = true;
        }

        if (ImGui::MenuItem("Delete Node(s)")) 
        {
            m_bDeleteSelectedNodeRequested = true;
        }

        ImGui::EndPopup();
    }

    if (bTreeNodeOpen)
    {
        for (std::vector<std::shared_ptr<SceneNode>>::const_iterator it = node->GetChildren().begin(); it != node->GetChildren().end(); it++)
        {
            ShowSceneNodeTreeNode((*it));
        }

        ImGui::TreePop();
    }
}

void Application::ShowSceneNodeEditor()
{
    if (!m_selectedSceneNode)
    {
        return;
    }

    Json selectedSceneNodeData = m_selectedSceneNode->ToJSON();

    if (ImGui::Begin("SceneNode Editor"))
    {
        std::string type = selectedSceneNodeData["type"].get<std::string>();
        Json selectedSceneNodeEditorData = m_sceneNodeEditorSceneNodeDefs.at(type);

        for (Json::iterator it = selectedSceneNodeEditorData["elements"].begin(); it != selectedSceneNodeEditorData["elements"].end(); it++)
        {
            std::string elementName = (*it).at("name").get<std::string>();

            uint8_t hashedElementType = m_sceneNodeEditorTypemap.at((*it)["type"]);

            switch (hashedElementType)
            {
                case SceneNodeEditorType_Integer:
                {
                    int v = 0;
                    v = selectedSceneNodeData[elementName].get<int>();

                    if (ImGui::DragInt(elementName.c_str(), &v, 0.5f))
                    {
                        selectedSceneNodeData[elementName] = v;
                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }

                case SceneNodeEditorType_Float:
                {
                    float v = 0;
                    v = selectedSceneNodeData[elementName].get<float>();

                    if (ImGui::DragFloat(elementName.c_str(), &v, 0.5f))
                    {
                        selectedSceneNodeData[elementName] = v;
                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }

                case SceneNodeEditorType_Vec3:
                {
                    glm::vec3 v(0, 0, 0);
                    v.x = selectedSceneNodeData[elementName]["x"].get<float>();
                    v.y = selectedSceneNodeData[elementName]["y"].get<float>();
                    v.z = selectedSceneNodeData[elementName]["z"].get<float>();

                    if (ImGui::DragFloat3(elementName.c_str(), glm::value_ptr(v), 0.5f))
                    {
                        selectedSceneNodeData[elementName]["x"] = v.x;
                        selectedSceneNodeData[elementName]["y"] = v.y;
                        selectedSceneNodeData[elementName]["z"] = v.z;

                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }

                case SceneNodeEditorType_String:
                {
                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    strncpy_s(buffer, selectedSceneNodeData[elementName].get<std::string>().data(), sizeof(buffer));

                    if (ImGui::InputText(elementName.c_str(), buffer, sizeof(buffer)))
                    {
                        selectedSceneNodeData[elementName] = std::string(buffer);
                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }

                case SceneNodeEditorType_RGB:
                {
                    glm::vec3 v(0, 0, 0);
                    v.x = selectedSceneNodeData[elementName]["x"].get<float>();
                    v.y = selectedSceneNodeData[elementName]["y"].get<float>();
                    v.z = selectedSceneNodeData[elementName]["z"].get<float>();  

                    if (ImGui::ColorEdit3(elementName.c_str(), glm::value_ptr(v)))
                    {
                        selectedSceneNodeData[elementName]["x"] = v.x;
                        selectedSceneNodeData[elementName]["y"] = v.y;
                        selectedSceneNodeData[elementName]["z"] = v.z;

                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }

                case SceneNodeEditorType_RGBA:
                {
                    glm::vec4 v(0, 0, 0, 0);
                    v.x = selectedSceneNodeData[elementName]["x"].get<float>();
                    v.y = selectedSceneNodeData[elementName]["y"].get<float>();
                    v.z = selectedSceneNodeData[elementName]["z"].get<float>();  
                    v.w = selectedSceneNodeData[elementName]["w"].get<float>();  

                    if (ImGui::ColorEdit4(elementName.c_str(), glm::value_ptr(v)))
                    {
                        selectedSceneNodeData[elementName]["x"] = v.x;
                        selectedSceneNodeData[elementName]["y"] = v.y;
                        selectedSceneNodeData[elementName]["z"] = v.z;
                        selectedSceneNodeData[elementName]["w"] = v.w;

                        m_selectedSceneNode->Init(selectedSceneNodeData);

                        m_selectedSceneNode->m_bChanged = true;
                    }

                    break;
                }
            }
        }    
    }

    ImGui::End();
}

void Application::LoadScene(const std::string& filename)
{
    std::fstream file(filename);
    Json sceneData = Json::parse(file);
    file.close();

    std::shared_ptr<PawnNode> root = m_pRenderer->GetSceneGraphRoot();

    for (Json::iterator it = sceneData["sceneNodeList"].begin(); it != sceneData["sceneNodeList"].end(); it++)
    {
        root->AddChild(m_sceneNodeFactory.CreateSceneNode((*it)));
    }

    root->PostInit();
}

static Json SaveSceneNode(std::shared_ptr<SceneNode> node)
{
    Json data = node->ToJSON();

    for (std::vector<std::shared_ptr<SceneNode>>::iterator it = node->GetChildren().begin(); it !=  node->GetChildren().end(); it++)
    {
        data["children"].push_back(SaveSceneNode((*it)));
    }

    return data;
}   

void Application::SaveScene(const std::string& filename)
{
    Json sceneData;
    sceneData["sceneNodeList"] = Json::array();
    
    std::shared_ptr<SceneNode> root = m_pRenderer->GetSceneGraphRoot();
    Json sceneGraphRootData = SaveSceneNode(root);

    for (Json::iterator it = sceneGraphRootData["children"].begin(); it != sceneGraphRootData["children"].end(); it++)
    {
        sceneData["sceneNodeList"].push_back((*it));
    }

    std::ofstream file(filename);
    file << sceneData.dump(4);
    file.close();
}