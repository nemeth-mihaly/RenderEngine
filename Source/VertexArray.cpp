#include "VertexArray.h"

//-----------------------------------------------------------------------------
// VertexArray Implementation
//-----------------------------------------------------------------------------

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_arrayID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_arrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_arrayID);
}

void VertexArray::SetVertexBuffer(GLuint binding, StrongVertexBufferPtr& vertexBuffer, GLsizei stride, GLuint inputRate)
{
    glVertexArrayVertexBuffer(m_arrayID, binding, vertexBuffer->m_bufferID, 0, stride);
    glVertexArrayBindingDivisor(m_arrayID, binding, inputRate);
}

void VertexArray::SetIndexBuffer(StrongIndexBufferPtr& indexBuffer)
{
    glVertexArrayElementBuffer(m_arrayID, indexBuffer->m_bufferID);
}

void VertexArray::SetVertexAttribute(GLuint binding, GLuint location, GLint size, GLenum type, GLuint offset)
{
    glEnableVertexArrayAttrib(m_arrayID, location);
    glVertexArrayAttribFormat(m_arrayID, location, size, type, GL_FALSE, offset);
    glVertexArrayAttribBinding(m_arrayID, location, binding);
}