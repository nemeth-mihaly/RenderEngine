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
    float Range;
    float Falloff;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    float Theta;
    float Phi;
};

////////////////////////////////////////////////////
//  class LightSceneNode
////////////////////////////////////////////////////

class LightSceneNode : public SceneNode
{
 public: 
    LightSceneNode(const LightProperties& props);
    virtual ~LightSceneNode();

    virtual void Create();
    virtual void Update(const float deltaTime);

    const LightProperties& GetLightProperties() const { return m_LightProps; }

 protected:
    LightProperties m_LightProps;
};

typedef std::vector<std::shared_ptr<LightSceneNode>> LightSceneNodeList;