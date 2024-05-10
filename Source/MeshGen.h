#pragma once

#include <vector>

#include "GLCommon.h"

extern std::vector<Vertex_Position> GenerateTriangle_Position();
extern std::vector<Vertex_Textured> GenerateTriangle_Textured();

extern std::vector<Vertex_Position> GenerateRectangle_Position();
extern std::vector<Vertex_Textured> GenerateRectangle_Textured();

extern std::vector<Vertex_Position> GenerateCube_Position();

extern std::vector<Vertex_Position> GenerateUVSphere_Position(uint32_t segmentCount, uint32_t ringCount);