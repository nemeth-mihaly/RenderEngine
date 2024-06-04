#include "VertexArray.h"

////////////////////////////////////////////////////
//  VertexBuffer Implementation
////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(ssize_t size, uint32_t usage)
    : m_Size(size)
{
    glCreateBuffers(1, &m_BufferID);
    glNamedBufferData(m_BufferID, size, NULL, usage);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_BufferID);
}

void VertexBuffer::MapMemory(int64_t offset, ssize_t size, const void* pData)
{
    glNamedBufferSubData(m_BufferID, offset, size, pData);
}

////////////////////////////////////////////////////
//  VertexArray Implementation
////////////////////////////////////////////////////

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_ArrayID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_ArrayID);
}

void VertexArray::SetVertexBuffer(uint32_t binding, StrongVertexBufferPtr& vertexBuffer, int stride, uint32_t inputRate)
{
    glVertexArrayVertexBuffer(m_ArrayID, binding, vertexBuffer->m_BufferID, 0, stride);
    glVertexArrayBindingDivisor(m_ArrayID, binding, inputRate);
}

void VertexArray::SetVertexAttribute(uint32_t binding, uint32_t location, int size, uint32_t type, uint32_t offset)
{
    glEnableVertexArrayAttrib(m_ArrayID, location);
    glVertexArrayAttribFormat(m_ArrayID, location, size, type, GL_FALSE, offset);
    glVertexArrayAttribBinding(m_ArrayID, location, binding);
}