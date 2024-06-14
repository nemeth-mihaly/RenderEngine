#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

class Scene;

//////////////////////////////////////////////////////
//  class SceneNode
//////////////////////////////////////////////////////

class SceneNode
{
public:
    std::string m_name;

    SceneNode();
    virtual ~SceneNode();

    virtual void Update(Scene* pScene, const float deltaTime);
    virtual void Render(Scene* pScene);

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    glm::vec3& GetPosition() { return m_Position; }

    void SetMaterial(const Material& material) { m_Material = material; }
    const Material& GetMaterial() const { return m_Material; }

protected:
    glm::mat4       m_WorldTransform;
    glm::vec3       m_Position;

    Material        m_Material;
};

//////////////////////////////////////////////////////
//  struct AlphaNode
//////////////////////////////////////////////////////

struct AlphaNode
{
    std::shared_ptr<SceneNode> Node;
};

//////////////////////////////////////////////////////
//  class CameraSceneNode
//////////////////////////////////////////////////////

class CameraNode : public SceneNode
{
public:
    CameraNode();
    virtual ~CameraNode();

    const glm::mat4& GetProjection() const { return m_Projection; }
    const glm::mat4& GetView() const { return m_View; }

    glm::mat4 WorldViewProjection();

    void SetForwardDir(const glm::vec3& forwardDir) { m_ForwardDir = forwardDir; }
    const glm::vec3& GetForwardDir() const { return m_ForwardDir; }

    glm::vec3 m_TargetPos;
    std::shared_ptr<SceneNode> m_TargetNode;

private:
    glm::mat4   m_View;
    glm::mat4   m_Projection;

    glm::vec3   m_ForwardDir;
};

//////////////////////////////////////////////////////
//  class MeshSceneNode
//////////////////////////////////////////////////////

class MeshNode : public SceneNode
{
public:
    MeshNode(const StrongMeshPtr& mesh, const StrongShaderPtr& shader, const StrongTexturePtr& texture);
    virtual ~MeshNode();

    virtual void Render(Scene* pScene);

private:
    StrongMeshPtr       m_Mesh;
    StrongShaderPtr     m_Shader;
    StrongTexturePtr    m_Texture;
};

//////////////////////////////////////////////////////
//  enum CubeMapSide
//////////////////////////////////////////////////////

enum CubeMapSide
{
    CubeMapSide_E       = 0,
    CubeMapSide_W       = 1,
    CubeMapSide_U       = 2,
    CubeMapSide_D       = 3,
    CubeMapSide_N       = 4,
    CubeMapSide_S       = 5,
    CubeMapSide_Count   = 6,
};

//////////////////////////////////////////////////////
//  class CubeMapNode
//////////////////////////////////////////////////////

class CubeMapNode : public SceneNode
{
public:
    CubeMapNode();
    virtual ~CubeMapNode();

    virtual void Render(Scene* pScene) override;

private:
    uint32_t                m_CubeMapSideVertCount;

    uint32_t                m_VertCount;

    StrongVertexArrayPtr    m_VertexArray;
    StrongVertexBufferPtr   m_VertexBuffer;

    StrongTexturePtr        m_Textures[CubeMapSide_Count];
};

//////////////////////////////////////////////////////
//  class BillboardNode
//////////////////////////////////////////////////////

class BillboardNode : public SceneNode
{
public:
    BillboardNode(const StrongTexturePtr& texture);
    virtual ~BillboardNode();

    virtual void Render(Scene* pScene) override;

private:
    uint32_t                m_IndexCount;

    StrongVertexArrayPtr    m_VertexArray;
    StrongVertexBufferPtr   m_VertexBuffer;
    StrongIndexBufferPtr    m_IndexBuffer;

    StrongTexturePtr        m_Texture;
};

//////////////////////////////////////////////////////
//  class TerrainNode
//////////////////////////////////////////////////////

class TerrainNode : public SceneNode
{
public:
    TerrainNode();
    virtual ~TerrainNode();

    virtual void Render(Scene* pScene) override;

    float HeightAt(float x, float z);

private:
    uint32_t                m_IndexCount;

    StrongVertexArrayPtr    m_VertexArray;
    StrongVertexBufferPtr   m_VertexBuffer;
    StrongIndexBufferPtr    m_IndexBuffer;

    int                     m_HeightMapWidth;
    int                     m_HeightMapHeight;

    std::vector<float>      m_HeightPointValues;
};