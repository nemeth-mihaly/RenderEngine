#include "VertexArray.h"

////////////////////////////////////////////////////
//  VertexArray Implementation
////////////////////////////////////////////////////

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_VertexArrayID);
}

VertexArray::~VertexArray()
{
    for (GLuint vertexBufferID : m_VertexBufferIDs)
    {
        glDeleteBuffers(1, &vertexBufferID);
    }

    glDeleteVertexArrays(1, &m_VertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_VertexArrayID);
}

GLuint VertexArray::AddVertexBuffer(GLenum usage, GLsizei stride, GLsizeiptr size, const void* pData)
{
    GLuint vertexBufferID;
    glCreateBuffers(1, &vertexBufferID);
    glNamedBufferData(vertexBufferID, size, pData, usage);

    glVertexArrayVertexBuffer(m_VertexArrayID, m_VertexBufferIDs.size(), vertexBufferID, 0, stride);
    m_VertexBufferIDs.push_back(vertexBufferID);

    return vertexBufferID;
}

void VertexArray::SetBufferSubData(GLuint bufferID, GLintptr offset, GLsizeiptr size, const void* pData) const
{
    glNamedBufferSubData(bufferID, offset, size, pData);
}

void VertexArray::SetAttribute(GLuint index, GLint size, GLenum type, GLuint relativeoffset, GLuint bindingindex) const
{
    glEnableVertexArrayAttrib(m_VertexArrayID, index);
    glVertexArrayAttribFormat(m_VertexArrayID, index, size, type, GL_FALSE, relativeoffset);
    glVertexArrayAttribBinding(m_VertexArrayID, index, bindingindex);
}