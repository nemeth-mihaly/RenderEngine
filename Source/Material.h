#pragma once

#include <cstddef>
#include <cstdint>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

////////////////////////////////////////////////////
//  class Material_t
////////////////////////////////////////////////////

class Material_t
{
 public:
    Material_t();

    glm::vec4       Ambient;
    glm::vec4       Diffuse;
    glm::vec4       Specular;
    glm::vec4       Emissive;
    float       SpecularPower;

    bool        bUseTexture;
};