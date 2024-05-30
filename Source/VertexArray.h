#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class VertexArray
{
 public:
    VertexArray();
    ~VertexArray();

    void Bind() const;

    GLuint AddVertexBuffer(GLsizei stride, GLsizeiptr size, const void* pData);
    void SetBufferSubData(GLuint bufferID, GLintptr offset, GLsizeiptr size, const void* pData) const;

    void SetAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset, GLuint bindingindex) const;

 private:
   GLuint m_VertexArrayID;
   std::vector<GLuint> m_VertexBufferIDs;
};