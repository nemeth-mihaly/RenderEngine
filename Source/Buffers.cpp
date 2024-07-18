#include "Buffers.h"

#include <cstdio>

//-----------------------------------------------------------------------------
// UniformBuffer Implementation
//-----------------------------------------------------------------------------

UniformBuffer::UniformBuffer()  
{
    m_size = 0;
    m_bufferID = 0xFFFFFFFF;
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void UniformBuffer::Init(GLuint binding, GLsizeiptr size, GLenum usage)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_bufferID);

    m_size = size;
}

void UniformBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}

//-----------------------------------------------------------------------------
// VertexBuffer Implementation
//-----------------------------------------------------------------------------

VertexBuffer::VertexBuffer()
{
    m_size = 0;
    m_bufferID = 0xFFFFFFFF;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void VertexBuffer::Init(GLsizeiptr size, GLenum usage)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);

    m_size = size;
}

void VertexBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}

//-----------------------------------------------------------------------------
// IndexBuffer Implementation
//-----------------------------------------------------------------------------

IndexBuffer::IndexBuffer()
{
    m_size = 0;
    m_bufferID = 0xFFFFFFFF;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_bufferID);
}

void IndexBuffer::Init(GLsizeiptr size, GLenum usage)
{
    glCreateBuffers(1, &m_bufferID);
    glNamedBufferData(m_bufferID, size, nullptr, usage);

    m_size = size;
}

void IndexBuffer::MapMemory(GLintptr offset, GLsizeiptr size, const void* pData)
{
    glNamedBufferSubData(m_bufferID, offset, size, pData);
}