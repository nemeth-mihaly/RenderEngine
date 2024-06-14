#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "Buffers.h"

//-----------------------------------------------------------------------------
// enum VertexArrayInputRate
//-----------------------------------------------------------------------------

enum VertexArrayInputRate
{
    VertexArrayInputRate_Vertex      = 0,
    VertexArrayInputRate_Instance    = 1
};

//-----------------------------------------------------------------------------
// class VertexArray
//-----------------------------------------------------------------------------

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;

    void SetVertexBuffer(GLuint binding, StrongVertexBufferPtr& vertexBuffer, GLsizei stride, GLuint inputRate);
    void SetIndexBuffer(StrongIndexBufferPtr& indexBuffer);

    void SetVertexAttribute(GLuint binding, GLuint location, GLint size, GLenum type, GLuint offset);

public:
   GLuint   m_arrayID;
};

typedef std::shared_ptr<VertexArray> StrongVertexArrayPtr;