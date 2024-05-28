#pragma once

#include "Common.h"

#include <vector>

#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class Scene;

////////////////////////////////////////////////////
//  class SceneNode
////////////////////////////////////////////////////

class SceneNode
{
 public:
    SceneNode();
    virtual ~SceneNode();

    virtual void Update(Scene* pScene, const float deltaTime);
    virtual void Render(Scene* pScene);

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

    void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    const glm::vec3& GetScale() const { return m_Scale; }

    void SetRotation(const glm::vec3& axis, float degrees) { m_Rotation = glm::vec4(axis, degrees); }
    const glm::vec4& GetRotation() const { return m_Rotation; }

    void SetMaterial(const Material& InMaterial) { Material = InMaterial; }
    const Material& GetMaterial() const { return Material; }

 protected:
    glm::vec3 m_Pos;
    glm::vec3 m_Scale;
    glm::vec4 m_Rotation;
    Material Material;
};

////////////////////////////////////////////////////
//  class CameraSceneNode
////////////////////////////////////////////////////

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

 private:
    glm::mat4 m_Projection;
    glm::mat4 m_View;

    glm::vec3 m_ForwardDir;
};

////////////////////////////////////////////////////
//  class MeshSceneNode
////////////////////////////////////////////////////

class MeshSceneNode : public SceneNode
{
 public:
    MeshSceneNode(const std::string& meshName, const std::string& shaderProgName, const std::string& textureName);

    virtual ~MeshSceneNode();

    virtual void Render(Scene* pScene);

 private:
    std::string m_MeshName;
    std::string m_ShaderProgName;
    std::string m_TextureName;
};

////////////////////////////////////////////////////
//  struct AlphaNode
////////////////////////////////////////////////////

struct AlphaNode
{
    std::shared_ptr<SceneNode> Node;
};

////////////////////////////////////////////////////
//  class SkyNode
////////////////////////////////////////////////////

class SkyNode : public SceneNode
{
 public:
    SkyNode();
    virtual ~SkyNode();

    virtual void Render(Scene* pScene) override;

 private:
    std::shared_ptr<VertexArray_t> VertexArray;
    std::shared_ptr<Texture_t> Textures[6];
};