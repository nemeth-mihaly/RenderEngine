#pragma once

#include "Common.h"

#include <vector>

constexpr float Pi = 3.14159265358979323846f;

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 Uv;
};

extern std::vector<Vertex> CreateTriangleVertices();
extern std::vector<Vertex> CreateRectangleVertices();
extern std::vector<Vertex> CreateCubeVertices();