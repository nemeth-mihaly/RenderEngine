#include "Lights.h"

////////////////////////////////////////////////////
//  LightSceneNode_t Implementation
////////////////////////////////////////////////////

LightSceneNode_t::LightSceneNode_t(const LightProperties_t& InProperties)
    : Properties(InProperties)
{
}

LightSceneNode_t::~LightSceneNode_t()
{
}

void LightSceneNode_t::Update(const float DeltaTime)
{
}