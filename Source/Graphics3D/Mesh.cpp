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

void Mesh::Create()
{
    m_VertexBuffer.reset(new VertexBuffer());
    assert(m_VertexBuffer);
    m_VertexBuffer->Create();
    m_VertexBuffer->SetBufferData(
        sizeof(Vertex) * VertexCount, 
        pVertices, 
        GL_STATIC_DRAW
    );

    m_VertexArray.reset(new VertexArray());
    assert(m_VertexArray);
    m_VertexArray->Create();
    m_VertexArray->SetVertexBuffer(m_VertexBuffer, sizeof(Vertex));
    m_VertexArray->SetAttribute(0, 3, GL_FLOAT, 0);
    m_VertexArray->SetAttribute(1, 3, GL_FLOAT, 12);
    m_VertexArray->SetAttribute(2, 2, GL_FLOAT, 24);
}