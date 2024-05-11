#pragma once

#include "Common.h"

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

 protected:
    glm::vec3 m_Pos;
};

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