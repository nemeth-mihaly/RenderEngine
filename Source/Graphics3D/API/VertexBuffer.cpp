#include "Graphics3D/API/VertexBuffer.h"

////////////////////////////////////////////////////
//  VertexBuffer Implementation
////////////////////////////////////////////////////

VertexBuffer::VertexBuffer()
{
    m_VertexBufferID = 0xFFFFFFFF;
}

VertexBuffer::~VertexBuffer()
{
    if (m_VertexBufferID != 0xFFFFFFFF)
        glDeleteBuffers(1, &m_VertexBufferID);
}

void VertexBuffer::Create()
{
    glCreateBuffers(1, &m_VertexBufferID);
}

void VertexBuffer::SetActive(GLboolean bActive) const
{
}

void VertexBuffer::SetBufferData(GLsizeiptr size, const void* pData, GLenum usage) const
{
    glNamedBufferData(m_VertexBufferID, size, pData, usage);
}

void VertexBuffer::SetBufferSubData(GLintptr offset, GLsizeiptr size, const void* pData) const
{
    glNamedBufferSubData(m_VertexBufferID, offset, size, pData);
}