#pragma once

#include "Common.h"

#include "Graphics3D/Buffer.h"

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class VertexArray
{
 public:
    VertexArray();
    ~VertexArray();

    void Create();
    void SetVertexBuffer(const StrongBufferPtr& buffer, GLsizei stride) const;
    void SetVertexInputAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset) const;
    void Bind() const;

 private:
    GLuint m_VertexArrayID;
};

typedef std::shared_ptr<VertexArray> StrongVertexArrayPtr;