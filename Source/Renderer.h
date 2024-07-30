#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <vector>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "Geometry.h"

#include "Shader.h"

#include "VertexArray.h"
#include "Buffers.h"

#include "Camera.h"
#include "World.h"
#include "SceneNodes.h"

extern bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius);

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
    void AddCircle(const glm::vec3& position, const glm::vec3& normal, float radius, const glm::vec3& color);
    void AddSphere(const glm::vec3& position, float radius, const glm::vec3& color);

    void Render(Shader* pShader);

    void ToggleDepthTest();

    int GetNumVerts() { return m_numVertsLastFrame; }
    int GetMaxVerts() { return 8192; }

 private:
    bool    m_bDepthEnabled;

    int m_numVertsLastFrame;
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

    LineRenderer* GetLineRenderer() { return &m_lineRenderer; }

    void AddLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, const glm::vec3& color);
    void AddCross(const glm::vec3& position, float size, const glm::vec3& color);
    void AddBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
    void AddCircle(const glm::vec3& position, const glm::vec3& normal, float radius, const glm::vec3& color);
    void AddSphere(const glm::vec3& position, float radius, const glm::vec3& color);

    void ToggleDepthTestForDebugPass() { m_lineRenderer.ToggleDepthTest(); }

    Shader* GetShader_UnlitColored() { return &m_shader_UnlitColored; }
    Shader* GetShader_UnlitColoredInstanced() { return &m_shader_UnlitColoredInstanced; }
    Shader* GetShader_UnlitTexturedColored() { return &m_shader_UnlitTexturedColored; }
    Shader* GetShader_UnlitTexturedColored2() { return &m_shader_UnlitTexturedColored2; }
    Shader* GetShader_LitTexturedColored() { return &m_shader_LitTexturedColored; }
    Shader* GetShader_UnlitTexturedColoredTerrain() { return &m_shader_UnlitColoredTexturedTerrain; }

    void SetWorld(std::shared_ptr<World> world) { m_world = world; }
    std::shared_ptr<World> GetWorld() { return m_world; }

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
    Shader                      m_shader_UnlitColoredTexturedTerrain;

    std::shared_ptr<World>      m_world;
    std::shared_ptr<PawnNode>   m_sceneGraphRoot;
    std::shared_ptr<Camera>     m_camera;

    std::vector<std::shared_ptr<SceneNode>> m_transparentSceneNodes; 
    std::vector<std::shared_ptr<LightNode>> m_lights;

    LineRenderer                m_lineRenderer;
};