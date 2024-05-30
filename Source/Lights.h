#pragma once

#include <cstddef>
#include <cstdint>

#include "SceneNodes.h"

class Scene;

////////////////////////////////////////////////////
//  enum class LightType
////////////////////////////////////////////////////

enum class LightType
{
    Directional = 0,
    Point = 1,
    Spot = 2,
};

////////////////////////////////////////////////////
//  struct LightProperties
////////////////////////////////////////////////////

struct LightProperties
{
    LightType Type;
    glm::vec3 Position;
    glm::vec3 Direction;
    float Range;
    float Falloff;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    float Theta;
    float Phi;
};

////////////////////////////////////////////////////
//  class LightNode
////////////////////////////////////////////////////

class LightNode : public SceneNode
{
 public: 
    LightNode(const LightProperties& properties);
    virtual ~LightNode();

    virtual void Update(Scene* pScene, const float deltaTime) override;

    const LightProperties& GetProperties() const { return m_Properties; }

 protected:
    LightProperties m_Properties;
};