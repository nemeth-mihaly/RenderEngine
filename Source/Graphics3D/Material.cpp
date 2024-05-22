#include "Graphics3D/Material.h"

////////////////////////////////////////////////////
//  Material Implementation
////////////////////////////////////////////////////

Material::Material()
{
    Ambient     = Color(0.10f, 0.10f, 0.10f, 1.0f),
    Diffuse     = Color(1.0f, 1.0f, 1.0f, 1.0f),
    Specular    = Color(0.0f, 0.0f, 0.0f, 0.0f),
    Emissive    = Color(0.0f, 0.0f, 0.0f, 0.0f),
    SpecularPower = 32.0f;

    bUseTexture = false;
}