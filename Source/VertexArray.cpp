#include "VertexArray.h"

#include <stdio.h>
#include <iostream>

//-----------------------------------------------------------------------------
// VertexArray Implementation
//-----------------------------------------------------------------------------

VertexArray::VertexArray()
{
    m_arrayID = 0xFFFFFFFF;
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_arrayID);
}

void VertexArray::Init()
{
    glCreateVertexArrays(1, &m_arrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_arrayID);
}

void VertexArray::SetVertexBuffer(GLuint binding, VertexBuffer* pVertexBuffer, GLsizei stride, GLuint inputRate)
{
    glVertexArrayVertexBuffer(m_arrayID, binding, pVertexBuffer->m_bufferID, 0, stride);
    
    if (inputRate == VertexArrayInputRate_Instance)
    {
        int maxAttribSize = sizeof(glm::vec4);
    
        int numDivisors = ((stride + maxAttribSize) - 1) / maxAttribSize;

        for (int i = 0; i < numDivisors; i++)
        {
            glVertexArrayBindingDivisor(m_arrayID, binding + i, inputRate);
        }
    }
}

void VertexArray::SetIndexBuffer(IndexBuffer* pIndexBuffer)
{
   glVertexArrayElementBuffer(m_arrayID, pIndexBuffer->m_bufferID);
}

void VertexArray::SetVertexAttribute(GLuint binding, GLuint location, GLint size, GLenum type, GLuint offset)
{
    glEnableVertexArrayAttrib(m_arrayID, location);
    glVertexArrayAttribFormat(m_arrayID, location, size, type, GL_FALSE, offset);
    glVertexArrayAttribBinding(m_arrayID, location, binding);
}