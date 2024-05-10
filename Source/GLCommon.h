#pragma once

#include "glad/glad_wgl.h"
#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

constexpr GLuint InvalidGpuID = 0xffffffff;

struct Vertex_Position
{
    glm::vec3 Pos;
};

struct Vertex_Textured
{
    glm::vec3 Pos;
    glm::vec2 UV;
};