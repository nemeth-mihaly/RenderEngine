#pragma once

#include "Common.h"

#include <vector>

#include "VertexArray.h"
#include "Mesh.h"
#include "Graphics3D/API/ProgramPipeline.h"
#include "Texture.h"
#include "Material.h"

////////////////////////////////////////////////////
//  class SceneNode_t
////////////////////////////////////////////////////

class SceneNode_t
{
 public:
    SceneNode_t();
    virtual ~SceneNode_t();

    virtual void Create();

    virtual void Update(const float deltaTime);
    virtual void Render();

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

    void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    const glm::vec3& GetScale() const { return m_Scale; }

    void SetRotation(const glm::vec3& axis, float degrees) { m_Rotation = glm::vec4(axis, degrees); }
    const glm::vec4& GetRotation() const { return m_Rotation; }

    void SetMaterial(const Material_t& InMaterial) { Material = InMaterial; }
    const Material_t& GetMaterial() const { return Material; }

 protected:
    glm::vec3 m_Pos;
    glm::vec3 m_Scale;
    glm::vec4 m_Rotation;
    Material_t Material;
};

typedef std::vector<std::shared_ptr<SceneNode_t>> SceneNodeList_t;

////////////////////////////////////////////////////
//  class CameraSceneNode
////////////////////////////////////////////////////

class CameraSceneNode : public SceneNode_t
{
 public:
    CameraSceneNode();
    virtual ~CameraSceneNode();

    virtual void Create();

    const glm::mat4& GetProjection() const { return m_Projection; }
    const glm::mat4& GetView() const { return m_View; }

    glm::mat4 WorldViewProjection();

    void SetForwardDir(const glm::vec3& forwardDir) { m_ForwardDir = forwardDir; }
    const glm::vec3& GetForwardDir() const { return m_ForwardDir; }

 private:
    glm::mat4 m_Projection;
    glm::mat4 m_View;

    glm::vec3 m_ForwardDir;
};

////////////////////////////////////////////////////
//  class MeshSceneNode
////////////////////////////////////////////////////

class MeshSceneNode : public SceneNode_t
{
 public:
    MeshSceneNode(const std::string& meshName, const std::string& shaderProgName, const std::string& textureName);

    virtual ~MeshSceneNode();

    virtual void Create();
    virtual void Render();

 private:
    std::string m_MeshName;
    std::string m_ShaderProgName;
    std::string m_TextureName;
};

////////////////////////////////////////////////////
//  struct AlphaSceneNode
////////////////////////////////////////////////////

struct AlphaSceneNode
{
    std::shared_ptr<SceneNode_t> Node;
};

typedef std::vector<AlphaSceneNode*> AlphaSceneList;

////////////////////////////////////////////////////
//  class SkySceneNode_t
////////////////////////////////////////////////////

class SkySceneNode_t : public SceneNode_t
{
 public:
    SkySceneNode_t();
    virtual ~SkySceneNode_t();

    virtual void Render() override;

 private:
    std::shared_ptr<VertexArray_t> VertexArray;
    std::shared_ptr<Texture_t> Textures[6];
};