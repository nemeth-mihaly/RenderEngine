#include "Components.h"

//-----------------------------------------------------------------------------
// TransformComponent Implementation
//-----------------------------------------------------------------------------

TransformComponent::TransformComponent() 
{ 
    printf("Creating TransformComponent\n"); 
    
    m_Pos = glm::vec3(0.0f, 0.0f, 0.0f); 
}

//-----------------------------------------------------------------------------
// MeshDrawComponent Implementation
//-----------------------------------------------------------------------------

MeshDrawComponent::MeshDrawComponent()
{
    printf("Creating MeshDrawComponent\n"); 
}