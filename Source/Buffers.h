#pragma once

#include <stddef.h>
#include <stdint.h>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// class UniformBuffer
//-----------------------------------------------------------------------------

class UniformBuffer
{
 public:
    UniformBuffer();
    ~UniformBuffer();

    void Init(GLuint binding, GLsizeiptr size, GLenum usage);

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

 private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};

//-----------------------------------------------------------------------------
// class VertexBuffer
//-----------------------------------------------------------------------------

class VertexBuffer
{
    friend class VertexArray;

 public:
    VertexBuffer();
    ~VertexBuffer();

    void Init(GLsizeiptr size, GLenum usage);

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

 private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};

//-----------------------------------------------------------------------------
// class IndexBuffer
//-----------------------------------------------------------------------------

class IndexBuffer
{
    friend class VertexArray;

 public:
    IndexBuffer();
    ~IndexBuffer();

    void Init(GLsizeiptr size, GLenum usage);

    void MapMemory(GLintptr offset, GLsizeiptr size, const void* pData);

 private:
    GLsizeiptr  m_size;
    GLuint      m_bufferID;
};