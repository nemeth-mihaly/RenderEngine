#include "Buffers.h"

////////////////////////////////////////////////////
//  UniformBuffer Implementation
////////////////////////////////////////////////////

UniformBuffer::UniformBuffer(uint32_t binding, ssize_t size, uint32_t usage)  
    : m_Size(size)
{
    glCreateBuffers(1, &m_BufferID);
    glNamedBufferData(m_BufferID, size, nullptr, usage);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_BufferID);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_BufferID);
}

void UniformBuffer::MapMemory(int64_t offset, ssize_t size, const void* pData)
{
    glNamedBufferSubData(m_BufferID, offset, size, pData);
}

////////////////////////////////////////////////////
//  VertexBuffer Implementation
////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(ssize_t size, uint32_t usage)
    : m_Size(size)
{
    glCreateBuffers(1, &m_BufferID);
    glNamedBufferData(m_BufferID, size, nullptr, usage);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_BufferID);
}

void VertexBuffer::MapMemory(int64_t offset, ssize_t size, const void* pData)
{
    glNamedBufferSubData(m_BufferID, offset, size, pData);
}