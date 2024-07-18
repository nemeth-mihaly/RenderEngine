#pragma once

#include <stddef.h>
#include <stdint.h>

#include "3rdParty/glm/glm.hpp"

//-----------------------------------------------------------------------------
// class Material
//-----------------------------------------------------------------------------

class Material
{
public:
    Material();

    glm::vec4   ambient;
    glm::vec4   diffuse;
    glm::vec4   specular;
    glm::vec4   emissive;
    float       specularPower;

    bool        bUseTexture;
};