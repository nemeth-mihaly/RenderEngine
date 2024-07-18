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
// class Line3DRenderer
//-----------------------------------------------------------------------------

class Line3DRenderer
{
 public:
    Line3DRenderer();

    void Init();

    void AddLine3D(const glm::vec3& from, const glm::vec3& to);

    void Render(Shader* pShader);

 private:
    int                         m_numVerts;
    VertexArray                 m_vertexArray;
    VertexBuffer                m_vertexBuffer;
    VertexBuffer                m_instVertexBuffer;
    std::vector<glm::mat4>      m_instStagingBuffer;
};

//-----------------------------------------------------------------------------
// class Box3DRenderer
//-----------------------------------------------------------------------------

class Box3DRenderer
{
 public:
    Box3DRenderer();

    void Init();

    void AddBox3D(const glm::vec3& position, const glm::vec3& size);

    void Render(Shader* pShader);

 private:
    int                         m_numVerts;
    VertexArray                 m_vertexArray;
    VertexBuffer                m_vertexBuffer;
    VertexBuffer                m_instVertexBuffer;
    std::vector<glm::mat4>      m_instStagingBuffer;
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

    void AddLine3D(const glm::vec3& from, const glm::vec3& to);
    void AddBox3D(const glm::vec3& position, const glm::vec3& size);

    Shader* GetShader_UnlitColored() { return &m_shader_UnlitColored; }
    Shader* GetShader_UnlitColoredInstanced() { return &m_shader_UnlitColoredInstanced; }
    Shader* GetShader_UnlitTexturedColored() { return &m_shader_UnlitTexturedColored; }
    Shader* GetShader_UnlitTexturedColored2() { return &m_shader_UnlitTexturedColored2; }
    Shader* GetShader_LitTexturedColored() { return &m_shader_LitTexturedColored; }

    std::shared_ptr<PawnNode> GetSceneGraphRoot() { return m_sceneGraphRoot; }
    std::shared_ptr<Camera> GetCamera() { return m_camera; }

    void AddLight(std::shared_ptr<SceneNode> node);
    void RemoveLight(std::shared_ptr<SceneNode> node);

    void OnResize(int width, int height);

 private:
    Shader                      m_shader_UnlitColored;
    Shader                      m_shader_UnlitColoredInstanced;
    Shader                      m_shader_UnlitTexturedColored;
    Shader                      m_shader_UnlitTexturedColored2;
    Shader                      m_shader_LitTexturedColored;

    std::shared_ptr<PawnNode>   m_sceneGraphRoot;
    std::shared_ptr<Camera>     m_camera;

    std::vector<std::shared_ptr<LightNode>>     m_lights;

    Line3DRenderer              m_line3DRenderer;
    Box3DRenderer               m_box3DRenderer;
};