#pragma once

#include "Common.h"
#include "Graphics3D/SceneNodes.h"

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
    glm::vec4 Ambient;
    glm::vec4 Diffuse;
    glm::vec4 Specular;
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
    LightNode();
    virtual ~LightNode();

    virtual void VCreate();
    virtual void VUpdate(const float deltaTime);

    void SetLightProperties(const LightProperties& properties) { m_LightProperties = properties; }
    const LightProperties& GetLightProperties() const { return m_LightProperties; }

 protected:
    LightProperties m_LightProperties;
};

typedef std::vector<std::shared_ptr<LightNode>> LightSceneNodeList;