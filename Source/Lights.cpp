#include "Lights.h"

#include "Scene.h"

////////////////////////////////////////////////////
//  LightNode Implementation
////////////////////////////////////////////////////

LightNode::LightNode(const LightProperties& properties)
    : m_Properties(properties)
{
}

LightNode::~LightNode()
{
}

void LightNode::Update(Scene& scene, const float deltaTime)
{
}