#include "Graphics3D/Mesh.h"

////////////////////////////////////////////////////
//  Mesh Implementation
////////////////////////////////////////////////////

Mesh::Mesh()
{
    VertexCount = 0;
    pVertices = nullptr;
    VertexBufferOffset = 0;
}

Mesh::~Mesh()
{
}