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

 private:
    glm::vec3 m_Pos;
};

typedef std::shared_ptr<SceneNode> StrongSceneNodePtr;