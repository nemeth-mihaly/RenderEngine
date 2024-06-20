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

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "3rdParty/glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include "3rdParty/glm/gtx/quaternion.hpp"

class Scene;

class SceneNode;
typedef std::vector<std::shared_ptr<SceneNode>> SceneNodeVector;

//-----------------------------------------------------------------------------
// class SceneNode
//-----------------------------------------------------------------------------

class SceneNode
{
    friend class Scene;

public:
    SceneNode();
    virtual ~SceneNode();

    virtual void Init(Scene& scene);

    virtual void Update(Scene& scene, const float deltaTime);

    virtual void Render(Scene& scene);
    void RenderChildren(Scene& scene);

    void AddChild(std::shared_ptr<SceneNode> node);
    const SceneNodeVector& GetChildren() const { return m_children; }

    const std::string& GetName() const { return m_name; }

    void SetPosition(const glm::vec3& pos) { m_pos = pos; }
    const glm::vec3& GetPosition() const { return m_pos; }

    void SetRotation(const glm::quat& rotation) { m_rotation = rotation; }
    const glm::quat& GetRotation() const { return m_rotation; }

    void SetScale(const glm::vec3& scale) { m_scale = scale; }
    const glm::vec3& GetScale() const { return m_scale; }

    void SetMaterial(const Material& material) { m_material = material; }
    const Material& GetMaterial() const { return m_material; }

protected:
    SceneNode*          m_pParent;
    SceneNodeVector     m_children;

    std::string         m_name;

    glm::mat4           m_transform;

    glm::vec3           m_pos;
    glm::quat           m_rotation;
    glm::vec3           m_scale;

    Material            m_material;
};

//-----------------------------------------------------------------------------
// struct AlphaSceneNode
//-----------------------------------------------------------------------------

struct AlphaSceneNode
{
    std::shared_ptr<SceneNode> node;
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

    virtual void Init(Scene& scene);
    
    virtual void Render(Scene& scene);

private:
    StrongMeshPtr       m_Mesh;
    StrongShaderPtr     m_Shader;
    StrongTexturePtr    m_Texture;

    std::string     m_meshResource;
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

    virtual void Render(Scene& scene) override;

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

    virtual void Render(Scene& scene) override;

    float scale;

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

    virtual void Render(Scene& scene) override;

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

//-----------------------------------------------------------------------------
// class Lamp Node
//-----------------------------------------------------------------------------

class LampNode : public SceneNode
{
public:
    LampNode();
    virtual ~LampNode();

    virtual void Init(Scene& scene);
    virtual void Update(Scene& scene, const float deltaTime);
    virtual void Render(Scene& scene);

private:
    std::shared_ptr<MeshNode>       m_lamp;

    float m_elapsedTimeInSeconds;
    float m_glowScale;
    std::shared_ptr<BillboardNode>  m_glow;
};