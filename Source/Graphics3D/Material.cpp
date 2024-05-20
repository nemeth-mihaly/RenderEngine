#include "Graphics3D/Material.h"

////////////////////////////////////////////////////
//  Material Implementation
////////////////////////////////////////////////////

Material::Material()
{
    Ambient     = glm::vec4(0.10f, 0.10f, 0.10f, 1.0f),
    Diffuse     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    Specular    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    Emissive    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    SpecularPower = 32.0f;

    bUseTexture = false;
}