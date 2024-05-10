#include "Graphics3D/VertexArray.h"

////////////////////////////////////////////////////
//  VertexArray Implementation
////////////////////////////////////////////////////

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::Create()
{
    glCreateVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::SetVertexBuffer(const StrongBufferPtr& buffer, GLsizei stride) const
{
    glVertexArrayVertexBuffer(m_VertexArrayID, 0, buffer->m_BufferID, 0, stride);
}

void VertexArray::SetVertexInputAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset) const
{
    glEnableVertexArrayAttrib(m_VertexArrayID, index);
    glVertexArrayAttribFormat(m_VertexArrayID, index, size, type, GL_FALSE, relativeoffset);
    glVertexArrayAttribBinding(m_VertexArrayID, index, 0);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_VertexArrayID);
}