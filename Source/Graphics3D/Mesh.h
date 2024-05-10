#pragma once

#include "Common.h"
#include "Graphics3D/Geometry.h"

////////////////////////////////////////////////////
//  class Mesh
////////////////////////////////////////////////////

class Mesh
{
 public:
    Mesh();
    ~Mesh();
 
 public:
    uint32_t VertexCount;
    Vertex* pVertices;
    uint32_t VertexBufferOffset;
};

typedef std::shared_ptr<Mesh> StrongMeshPtr;