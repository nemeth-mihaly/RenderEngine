#pragma once

#include "Common.h"

////////////////////////////////////////////////////
//  class VertexBuffer
////////////////////////////////////////////////////

class VertexBuffer
{
    friend class VertexArray;

 public:
    VertexBuffer();
    ~VertexBuffer();

    void Create();
    void SetActive(GLboolean bActive = GL_TRUE) const;
    void SetBufferData(GLsizeiptr size, const void* pData, GLenum usage) const;
    void SetBufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const;

 private:
    GLuint      m_VertexBufferID;
};

typedef std::shared_ptr<VertexBuffer> StrongVertexBufferPtr;