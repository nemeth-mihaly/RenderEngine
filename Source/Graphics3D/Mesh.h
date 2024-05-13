#pragma once

#include "Common.h"
#include "Graphics3D/Buffer.h"
#include "Graphics3D/VertexArray.h"
#include "Graphics3D/Geometry.h"

////////////////////////////////////////////////////
//  class Mesh
////////////////////////////////////////////////////

class Mesh
{
 public:
    Mesh();
    ~Mesh();

    void Create();

 public:
    StrongVertexArrayPtr m_VertexArray;
    StrongBufferPtr m_VertexBuffer;
    uint32_t VertexCount;
    Vertex* pVertices;
    uint32_t VertexBufferOffset;
};

typedef std::shared_ptr<Mesh> StrongMeshPtr;