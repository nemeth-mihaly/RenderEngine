#include "Material.h"

//-----------------------------------------------------------------------------
// Material Implementation
//-----------------------------------------------------------------------------

Material::Material()
{
    ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
    diffuse = glm::vec4(1, 1, 1, 1),
    specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
    emissive = glm::vec4(0, 0, 0, 0),
    specularPower = 32.0f;

    bUseTexture = false;
}