#pragma once

#include "Common.h"

typedef     glm::vec4       Color;

////////////////////////////////////////////////////
//  class Material
////////////////////////////////////////////////////

class Material
{
 public:
    Material();

    Color       Ambient;
    Color       Diffuse;
    Color       Specular;
    Color       Emissive;
    float       SpecularPower;

    bool        bUseTexture;
};