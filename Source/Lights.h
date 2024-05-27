#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "Graphics3D/SceneNodes.h"

////////////////////////////////////////////////////
//  enum class LightType_t
////////////////////////////////////////////////////

enum class LightType_t
{
    Directional = 0,
    Point       = 1,
    Spot        = 2,
};

////////////////////////////////////////////////////
//  struct LightProperties_t
////////////////////////////////////////////////////

/** From: https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dlight9 */

struct LightProperties_t
{
    LightType_t     Type;
    glm::vec3       Position;
    glm::vec3       Direction;
    float           Range;
    float           Falloff;
    float           ConstantAttenuation;
    float           LinearAttenuation;
    float           QuadraticAttenuation;
    float           Theta;
    float           Phi;
};

////////////////////////////////////////////////////
//  class LightSceneNode_t
////////////////////////////////////////////////////

class LightSceneNode_t : public SceneNode_t
{
 public: 
    LightSceneNode_t(const LightProperties_t& InProperties);
    virtual ~LightSceneNode_t();

    virtual void Update(const float DeltaTime) override;

    const LightProperties_t& GetProperties() const { return Properties; }

 protected:
    LightProperties_t Properties;
};

typedef std::vector<std::shared_ptr<LightSceneNode_t>>  LightSceneNodeList_t;