#pragma once

#include "Common.h"

#include "Graphics3D/API/VertexBuffer.h"

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class VertexArray
{
 public:
    VertexArray();
    ~VertexArray();

    void Create();
    void SetActive(GLboolean bActive = GL_TRUE) const;
    void SetVertexBuffer(StrongVertexBufferPtr& vbo, GLsizei stride) const;
    void SetAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset) const;

 private:
    GLuint      m_VertexArrayID;
};

typedef std::shared_ptr<VertexArray> StrongVertexArrayPtr;