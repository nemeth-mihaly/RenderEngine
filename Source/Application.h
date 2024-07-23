#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <unordered_map>

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"

#include "3rdParty/nlohmann/json.hpp"
using Json = nlohmann::json;

#include "Renderer.h"
#include "Camera.h"

#include "SceneNodes.h"
#include "SceneNodeFactory.h"

#include "Mesh.h"
#include "Texture.h"

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
 public:
    Application();
    ~Application();

    bool Init(int width, int height);
    void RunLoop();

    const glm::ivec2& GetWindowSize() const { return m_windowSize; }

    Renderer* GetRenderer() { return m_pRenderer; }

    const glm::ivec2& GetMousePosition() const { return m_currentMousePos; }
    void ShowMouse();
    void HideMouse();

    std::shared_ptr<Mesh> GetMesh(const std::string& asset);
    std::shared_ptr<Texture> GetTexture(const std::string& asset);

    void OnClose();
    void OnResize(int width, int height);

    void OnKeyDown(int key);
    void OnKeyUp(int key);

    void OnMouseMove(int x, int y);
    void OnMouseButtonDown(int button);
    void OnMouseButtonUp(int button);

 private:
    void ShowSceneNodeTree();
    void ShowSceneNodeTreeNode(std::shared_ptr<SceneNode> node);
    void ShowSceneNodeEditor();

    void LoadScene(const std::string& filename);
    void SaveScene(const std::string& filename);

 private:
    bool    m_bRunning;
    bool    m_bWindowedMode;
    bool    m_bMinimized;
    
    glm::ivec2      m_windowPos;
    glm::ivec2      m_windowSize;
    GLFWwindow*     m_pWindow;

    glm::ivec2      m_currentMousePos;

    float       m_deltaTime;

    Renderer*           m_pRenderer;
    SceneNodeFactory    m_sceneNodeFactory;

    std::shared_ptr<Camera>     m_camera;
    std::shared_ptr<Camera>     m_camera2;

    CameraController*   m_pCameraController;
    CameraController    m_cameraController;
    CameraController    m_cameraController2;

    std::unordered_map<std::string, std::shared_ptr<Mesh>>      m_meshAssets;
    std::unordered_map<std::string, std::shared_ptr<Texture>>   m_textureAssets;

    bool    m_bAddChildSelectedNodeRequested;
    bool    m_bDeleteSelectedNodeRequested;
    std::shared_ptr<SceneNode>                  m_selectedSceneNode;
    std::unordered_map<std::string, Json>       m_sceneNodeEditorSceneNodeDefs;
    std::unordered_map<std::string, uint8_t>    m_sceneNodeEditorTypemap;
};

extern Application* g_pApp;