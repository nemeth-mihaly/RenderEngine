#pragma once

#include "Common.h"

#include <vector>

#include "Graphics3D/VertexArray.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/ShaderProgram.h"
#include "Graphics3D/Texture.h"

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
    virtual void VRender(StrongShaderProgPtr& shader);

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

 protected:
    glm::vec3 m_Pos;
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
    // FIXME: It's impossible to determine which mesh is uploaded to which vertex buffers without looking it up.
    MeshNode(const StrongVertexArrayPtr& vertexArray, const StrongMeshPtr& mesh, const StrongTexturePtr& texture);
    virtual ~MeshNode();

    virtual void VCreate();

    // FIXME: Kurva OpenGL.
    virtual void VRender(StrongShaderProgPtr& shader);

 private:
    StrongVertexArrayPtr m_VertexArray;
    StrongMeshPtr m_Mesh;
    StrongTexturePtr m_Texture;
};