#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <vector>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "3rdParty/glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include "3rdParty/glm/gtx/quaternion.hpp"

#include "Shader.h"

#include "VertexArray.h"
#include "Buffers.h"

#include "Camera.h"

#include "SceneNodes.h"

//-----------------------------------------------------------------------------
// class LineRenderer
//-----------------------------------------------------------------------------

class LineRenderer
{
 public:
    LineRenderer();

    void Init();

    void AddLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, const glm::vec3& color);
    void AddCross(const glm::vec3& position, float size, const glm::vec3& color);
    void AddBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);

    void Render(Shader* pShader);

    void ToggleDepthTest();

 private:
    bool    m_bDepthEnabled;

    std::vector<Vertex_UnlitColored> m_stagingBuffer;

    VertexArray                 m_vertexArray;
    VertexBuffer                m_vertexBuffer;
};

//-----------------------------------------------------------------------------
// class Renderer
//-----------------------------------------------------------------------------

class Renderer
{
 public:
    Renderer();
    ~Renderer();

    void Init();
    void Update(const float deltaTime);
    void Render();

    void AddLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, const glm::vec3& color);
    void AddCross(const glm::vec3& position, float size, const glm::vec3& color);
    void AddBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);

    void ToggleDepthTestForDebugPass() { m_lineRenderer.ToggleDepthTest(); }

    Shader* GetShader_UnlitColored() { return &m_shader_UnlitColored; }
    Shader* GetShader_UnlitColoredInstanced() { return &m_shader_UnlitColoredInstanced; }
    Shader* GetShader_UnlitTexturedColored() { return &m_shader_UnlitTexturedColored; }
    Shader* GetShader_UnlitTexturedColored2() { return &m_shader_UnlitTexturedColored2; }
    Shader* GetShader_LitTexturedColored() { return &m_shader_LitTexturedColored; }

    std::shared_ptr<PawnNode> GetSceneGraphRoot() { return m_sceneGraphRoot; }

    void SetCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> GetCamera() { return m_camera; }

    void AddLight(std::shared_ptr<SceneNode> node);
    void RemoveLight(std::shared_ptr<SceneNode> node);

    void AddTransparentSceneNode(std::shared_ptr<SceneNode> node);

    void OnResize(int width, int height);

 private:
    Shader                      m_shader_UnlitColored;
    Shader                      m_shader_UnlitColoredInstanced;
    Shader                      m_shader_UnlitTexturedColored;
    Shader                      m_shader_UnlitTexturedColored2;
    Shader                      m_shader_LitTexturedColored;

    std::shared_ptr<PawnNode>   m_sceneGraphRoot;
    std::shared_ptr<Camera>     m_camera;

    std::vector<std::shared_ptr<SceneNode>> m_transparentSceneNodes; 
    std::vector<std::shared_ptr<LightNode>> m_lights;

    LineRenderer                m_lineRenderer;
};