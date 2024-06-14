#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// class UniformBuffer
//-----------------------------------------------------------------------------

class UniformBuffer
{
public:
    UniformBuffer(GLuint binding, GLsizeiptr size, GLenum usage);
    ~UniformBuffer();

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};

typedef std::shared_ptr<UniformBuffer> StrongUniformBufferPtr;

//-----------------------------------------------------------------------------
// class VertexBuffer
//-----------------------------------------------------------------------------

class VertexBuffer
{
    friend class VertexArray;

public:
    VertexBuffer(GLsizeiptr size, GLenum usage);
    ~VertexBuffer();

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};

typedef std::shared_ptr<VertexBuffer> StrongVertexBufferPtr;

//-----------------------------------------------------------------------------
// class IndexBuffer
//-----------------------------------------------------------------------------

class IndexBuffer
{
    friend class VertexArray;

public:
    IndexBuffer(GLsizeiptr size, GLenum usage);
    ~IndexBuffer();

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};

typedef std::shared_ptr<IndexBuffer> StrongIndexBufferPtr;