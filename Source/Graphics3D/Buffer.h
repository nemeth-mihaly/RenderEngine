#pragma once

#include "Common.h"

////////////////////////////////////////////////////
//  class Buffer
////////////////////////////////////////////////////

class Buffer
{
    friend class VertexArray;

 public:
    Buffer();
    ~Buffer();

    void Create();
    void SetData(GLsizeiptr size, const void* pData, GLenum usage) const;
    void SetSubData(GLintptr offset, GLsizeiptr size, const void* data) const;

 private:
    GLuint m_BufferID;
};

typedef std::shared_ptr<Buffer> StrongBufferPtr;