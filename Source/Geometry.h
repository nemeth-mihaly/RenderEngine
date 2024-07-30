#pragma once

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "3rdParty/glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include "3rdParty/glm/gtx/quaternion.hpp"

//-----------------------------------------------------------------------------
// struct Ray
//-----------------------------------------------------------------------------

struct Ray
{
    glm::vec3   origin;
    glm::vec3   direction;
};