#include "Graphics3D/SceneNodes.h"

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::VCreate()
{
}

void SceneNode::VUpdate(const float deltaTime)
{
}

void SceneNode::VRender()
{
}

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

CameraNode::CameraNode()
{
    m_ForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
}

CameraNode::~CameraNode()
{
}

void CameraNode::VCreate()
{
    m_Projection = glm::perspective(glm::radians(45.0f), (1280 / static_cast<float>(720)), 0.001f, 1000.0f);
}

glm::mat4 CameraNode::WorldViewProjection()
{
    m_View = glm::lookAt(m_Pos, (m_Pos + m_ForwardDir), glm::vec3(0.0f, 1.0f, 0.0f));
    return (m_Projection * m_View);
}