#pragma once

#include "Common.h"

#include <vector>

#include "Graphics3D/VertexArray.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/ShaderProgram.h"
#include "Graphics3D/Texture.h"

////////////////////////////////////////////////////
//  struct Material
////////////////////////////////////////////////////

struct Material
{
    glm::vec3 Diffuse;
    glm::vec3 Ambient;
    glm::vec3 Specular;
    glm::vec3 Emissive;
    float Power;
};

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
 #if 0
    virtual void VRender(StrongShaderProgPtr& shader);
 #endif

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

    void SetScale(const glm::vec3& scale) { m_Scale = scale; }
    const glm::vec3& GetScale() const { return m_Scale; }

    Material& GetMaterial() { return m_Material; }

 protected:
    glm::vec3 m_Pos;
    glm::vec3 m_Scale;
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
 #if 0
    // FIXME: It's impossible to determine which mesh is uploaded to which vertex buffers without looking it up.
    MeshNode(const StrongVertexArrayPtr& vertexArray, const StrongMeshPtr& mesh, const StrongTexturePtr& texture);
#endif

    MeshNode(const StrongMeshPtr& mesh, const StrongShaderProgPtr& shader, const StrongTexturePtr& texture);

    virtual ~MeshNode();

    virtual void VCreate();

    virtual void VRender();
 #if 0
    // FIXME: Kurva OpenGL.
    virtual void VRender(StrongShaderProgPtr& shader);
 #endif

 private:
    StrongMeshPtr m_Mesh;
    StrongShaderProgPtr m_Shader;
    StrongTexturePtr m_Texture;

 #if 0
    StrongVertexArrayPtr m_VertexArray;
    StrongTexturePtr m_Texture;
 #endif
};

////////////////////////////////////////////////////
//  class LightNode
////////////////////////////////////////////////////

class LightNode : public SceneNode
{
 public: 
    LightNode(const StrongMeshPtr& mesh, const StrongShaderProgPtr& shader);
    virtual ~LightNode();

    virtual void VCreate();

    virtual void VUpdate(const float deltaTime);
 
    virtual void VRender();

 protected:
    StrongMeshPtr m_Mesh;
    StrongShaderProgPtr m_Shader;
};

typedef std::vector<std::shared_ptr<LightNode>> LightSceneNodeList;