#include "Graphics3D/Buffer.h"

////////////////////////////////////////////////////
//  Buffer Implementation
////////////////////////////////////////////////////

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_BufferID);
}

void Buffer::Create()
{
    glCreateBuffers(1, &m_BufferID);
}

void Buffer::SetData(GLsizeiptr size, const void* pData, GLenum usage) const
{
    glNamedBufferData(m_BufferID, size, pData, usage);
}

void Buffer::SetSubData(GLintptr offset, GLsizeiptr size, const void* pData) const
{
    glNamedBufferSubData(m_BufferID, offset, size, pData);
}