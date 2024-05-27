#pragma once

#include "Common.h"

#include <vector>

constexpr float Pi = 3.14159265358979323846f;

struct Vertex_t
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Texcoord;
};