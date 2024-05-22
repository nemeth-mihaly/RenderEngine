#include "Graphics3D/API/VertexArray.h"

////////////////////////////////////////////////////
//  VertexArray Implementation
////////////////////////////////////////////////////

VertexArray::VertexArray()
{
    m_VertexArrayID = 0xFFFFFFFF;
}

VertexArray::~VertexArray()
{
    if (m_VertexArrayID != 0xFFFFFFFF)
        glDeleteVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::Create()
{
    glCreateVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::SetActive(GLboolean bActive) const
{
    glBindVertexArray((bActive ? m_VertexArrayID : 0));
}

void VertexArray::SetVertexBuffer(StrongVertexBufferPtr& vbo, GLsizei stride) const
{
    glVertexArrayVertexBuffer(m_VertexArrayID, 0, vbo->m_VertexBufferID, 0, stride);
}

void VertexArray::SetAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset) const
{
    glEnableVertexArrayAttrib(m_VertexArrayID, index);
    glVertexArrayAttribFormat(m_VertexArrayID, index, size, type, GL_FALSE, relativeoffset);
    glVertexArrayAttribBinding(m_VertexArrayID, index, 0);
}