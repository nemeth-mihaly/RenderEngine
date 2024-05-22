#pragma once

#include "Common.h"
#include "Graphics3D/API/VertexBuffer.h"
#include "Graphics3D/API/VertexArray.h"
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
    StrongVertexBufferPtr m_VertexBuffer;
    uint32_t VertexCount;
    Vertex* pVertices;
    uint32_t VertexBufferOffset;
};

typedef std::shared_ptr<Mesh> StrongMeshPtr;