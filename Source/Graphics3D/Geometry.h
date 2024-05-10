#pragma once

#include "Common.h"

#include <vector>

////////////////////////////////////////////////////
//  Vertex Type Definitions
////////////////////////////////////////////////////

struct Vertex_Position
{
    glm::vec3 Pos;
    glm::vec3 Normal;
};

struct Vertex_Textured
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 Uv;
};


extern std::vector<Vertex_Position> CreateTriangle_Position();
extern std::vector<Vertex_Textured> CreateTriangle_Textured();

extern std::vector<Vertex_Position> CreateRectangle_Position();
extern std::vector<Vertex_Textured> CreateRectangle_Textured();

extern std::vector<Vertex_Position> CreateCube_Position();

extern std::vector<Vertex_Position> CreateUvSphere_Position(uint32_t segmentCount, uint32_t ringCount);