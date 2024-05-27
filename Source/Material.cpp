#include "Material.h"

////////////////////////////////////////////////////
//  Material_t Implementation
////////////////////////////////////////////////////

Material_t::Material_t()
{
    Ambient     = glm::vec4(0.10f, 0.10f, 0.10f, 1.0f),
    Diffuse     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    Specular    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    Emissive    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    SpecularPower = 32.0f;

    bUseTexture = false;
}