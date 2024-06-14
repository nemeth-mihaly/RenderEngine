#include "Buffers.h"

//-----------------------------------------------------------------------------
// UniformBuffer Implementation
//-----------------------------------------------------------------------------

UniformBuffer::UniformBuffer(GLuint binding, GLsizeiptr size, GLenum usage)  
    : m_size(size)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_bufferID);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void UniformBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}

//-----------------------------------------------------------------------------
// VertexBuffer Implementation
//-----------------------------------------------------------------------------

VertexBuffer::VertexBuffer(GLsizeiptr size, GLenum usage)
    : m_size(size)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void VertexBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}

//-----------------------------------------------------------------------------
// IndexBuffer Implementation
//-----------------------------------------------------------------------------

IndexBuffer::IndexBuffer(GLsizeiptr size, GLenum usage)
    : m_size(size)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void IndexBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}