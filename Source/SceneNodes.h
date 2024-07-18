#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

#include "3rdParty/nlohmann/json.hpp"
using Json = nlohmann::json;

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "3rdParty/glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include "3rdParty/glm/gtx/quaternion.hpp"

#include "Shader.h"

#include "VertexArray.h"
#include "Buffers.h"

#include "Material.h"

//-----------------------------------------------------------------------------
// class SceneNode
//-----------------------------------------------------------------------------

class SceneNode
{
    friend class Renderer;

 public:
    SceneNode();
    virtual ~SceneNode();

    virtual void Init(const Json& data);
    virtual void PostInit();
    virtual void Update(const float deltaTime);
    virtual void Render();
    void RenderChildren();

    virtual Json ToJSON();

    virtual const char* GetType() = 0;

    void AddChild(std::shared_ptr<SceneNode> node);
    void RemoveChild(std::shared_ptr<SceneNode> node);
    SceneNode* GetParent() { return m_pParent; }
    std::vector<std::shared_ptr<SceneNode>>& GetChildren() { return m_children; }

    const std::string& GetName() const { return m_name; }

 protected:
    SceneNode* m_pParent;
    std::vector<std::shared_ptr<SceneNode>> m_children;

    std::string     m_name;

    glm::mat4       m_transform;

    glm::vec3       m_position;
    glm::quat       m_rotation;
    glm::vec3       m_size;

    Material        m_material;
};

//-----------------------------------------------------------------------------
// class PawnNode
//-----------------------------------------------------------------------------

class PawnNode : public SceneNode
{
 public:
    static const char* g_type;

    PawnNode();
    virtual ~PawnNode();

    virtual void Init(const Json& data);
    virtual void PostInit();
    virtual void Update(const float deltaTime);
    virtual void Render();

    virtual Json ToJSON() override;

    virtual const char* GetType() override { return g_type; }
};

//-----------------------------------------------------------------------------
// class TestNode
//-----------------------------------------------------------------------------

class TestNode : public SceneNode
{
 public:
    static const char* g_type;

    TestNode();
    virtual ~TestNode();

    virtual void Init(const Json& data)         override;
    virtual void PostInit()                     override;
    virtual void Update(const float deltaTime)  override;
    virtual void Render()                       override;

    virtual Json ToJSON() override;

    virtual const char* GetType() override { return g_type; }

 private:
    glm::vec3       m_testElement;

    std::string     m_meshAsset;
    std::string     m_textureAsset;

    int             m_numVerts;
    VertexArray     m_vertexArray;
    VertexBuffer    m_vertexBuffer;
};

//-----------------------------------------------------------------------------
// class MeshNode
//-----------------------------------------------------------------------------

class MeshNode : public SceneNode
{
 public:
    static const char* g_type;
    
    MeshNode();
    virtual ~MeshNode();

    virtual void Init(const Json& data)         override;
    virtual void PostInit()                     override;
    virtual void Update(const float deltaTime)  override;
    virtual void Render()                       override;

    virtual Json ToJSON() override;

    virtual const char* GetType() override { return g_type; }

 protected:
    std::string     m_meshAsset;
    std::string     m_textureAsset;

    int             m_numVerts;
    VertexArray     m_vertexArray;
    VertexBuffer    m_vertexBuffer;
};

//-----------------------------------------------------------------------------
// enum LightType
//-----------------------------------------------------------------------------

enum LightType
{
    LightType_Directional = 0,
    LightType_Point = 1,
    LightType_Spot = 2,
};

//-----------------------------------------------------------------------------
// struct LightNode
//-----------------------------------------------------------------------------

struct LightProperties
{
    int         type;
    glm::vec3   position;
    float       padding1;
    glm::vec3   direction;
    float       range;
    float       fallOff;
    float       constantAttenuation;
    float       linearAttenuation;
    float       quadraticAttenuation;
    float       theta;
    float       phi;
};

//-----------------------------------------------------------------------------
// class LightNode
//-----------------------------------------------------------------------------

class LightNode : public SceneNode
{
 public:
    static const char* g_type;
    
    LightNode();
    virtual ~LightNode();

    virtual void Init(const Json& data)         override;
    virtual void PostInit()                     override;
    virtual void Update(const float deltaTime)  override;
    virtual void Render()                       override;

    virtual Json ToJSON() override;

    virtual const char* GetType() override { return g_type; }

    const LightProperties& GetProperties() const { return m_lightProperties; }

 protected:
    LightProperties     m_lightProperties;
};

/*

//-----------------------------------------------------------------------------
// class SceneNode
//-----------------------------------------------------------------------------

class SceneNode
{
    friend class Scene;

public:
    bool                m_bVisible = true;

    SceneNode();
    virtual ~SceneNode();

    virtual void Init(Scene& scene);

    virtual void Update(Scene& scene, const float deltaTime);

    virtual void Render(Scene& scene);
    void RenderChildren(Scene& scene);

    void AddChild(std::shared_ptr<SceneNode> node);
    const SceneNodeVector& GetChildren() const { return m_children; }
};

//-----------------------------------------------------------------------------
// struct AlphaSceneNode
//-----------------------------------------------------------------------------

struct AlphaSceneNode
{
    std::shared_ptr<SceneNode> node;
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

    virtual void Init(Scene& scene) override;
    virtual void Render(Scene& scene) override;

private:
    Shader  m_shader;
    size_t  m_numVerts;

    uint32_t                m_CubeMapSideVertCount;

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
    Shader m_shader;

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
    Shader m_shader;

    uint32_t                m_IndexCount;

    StrongVertexArrayPtr    m_VertexArray;
    StrongVertexBufferPtr   m_VertexBuffer;
    StrongIndexBufferPtr    m_IndexBuffer;

    int                     m_HeightMapWidth;
    int                     m_HeightMapHeight;

    std::vector<float>      m_HeightPointValues;
};

*/