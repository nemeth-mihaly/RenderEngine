#include "Material.h"

//////////////////////////////////////////////////////
//  Material Implementation
//////////////////////////////////////////////////////

Material::Material()
{
    Ambient     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    Diffuse     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    Specular    = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
    Emissive    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
    SpecularPower = 32.0f;

    bUseTexture = false;
}

Material::~Material()
{
}