#pragma once

#include "Common.h"

#include <vector>

#include "Graphics3D/API/VertexArray.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/API/ProgramPipeline.h"
#include "Graphics3D/API/Texture.h"
#include "Graphics3D/Material.h"

////////////////////////////////////////////////////
//  class SceneNode
////////////////////////////////////////////////////

class SceneNode
{
 public:
    SceneNode();
    virtual ~SceneNode();

    virtual void VCreate();

    virtual void VUpdate(const float deltaTime);
    virtual void VRender();

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

    void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    const glm::vec3& GetScale() const { return m_Scale; }

    void SetRotation(const glm::vec3& axis, float degrees) { m_Rotation = glm::vec4(axis, degrees); }
    const glm::vec4& GetRotation() const { return m_Rotation; }

    Material& GetMaterial() { return m_Material; }

 protected:
    glm::vec3 m_Pos;
    glm::vec3 m_Scale;
    glm::vec4 m_Rotation;
    Material m_Material;
};

typedef std::vector<std::shared_ptr<SceneNode>> SceneNodeList;

////////////////////////////////////////////////////
//  class CameraNode
////////////////////////////////////////////////////

class CameraNode : public SceneNode
{
 public:
    CameraNode();
    virtual ~CameraNode();

    virtual void VCreate();

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
//  class MeshNode
////////////////////////////////////////////////////

class MeshNode : public SceneNode
{
 public:
    MeshNode(const std::string& meshName, const std::string& shaderProgName, const std::string& textureName);

    virtual ~MeshNode();

    virtual void VCreate();
    virtual void VRender();

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
    std::shared_ptr<SceneNode> Node;
};

typedef std::vector<AlphaSceneNode*> AlphaSceneList;