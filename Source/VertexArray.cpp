#include "VertexArray.h"

////////////////////////////////////////////////////
//  VertexArray_t Implementation
////////////////////////////////////////////////////

VertexArray_t::VertexArray_t(const void* InVerticesData, const uint32_t InNumVertices)
    : NumVertices(InNumVertices)
{
    glCreateBuffers(1, &VertexBuffer);
    glNamedBufferData(VertexBuffer, (sizeof(float) * 8) * InNumVertices, InVerticesData, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &VertexArray);

    glVertexArrayVertexBuffer(VertexArray, 0, VertexBuffer, 0, sizeof(float) * 8);

    glEnableVertexArrayAttrib(VertexArray, 0);
    glVertexArrayAttribFormat(VertexArray, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 0);
    glVertexArrayAttribBinding(VertexArray, 0, 0);

    glEnableVertexArrayAttrib(VertexArray, 1);
    glVertexArrayAttribFormat(VertexArray, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glVertexArrayAttribBinding(VertexArray, 1, 0);

    glEnableVertexArrayAttrib(VertexArray, 2);
    glVertexArrayAttribFormat(VertexArray, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6);
    glVertexArrayAttribBinding(VertexArray, 2, 0);
}

VertexArray_t::~VertexArray_t()
{
    glDeleteVertexArrays(1, &VertexArray);
    glDeleteBuffers(1, &VertexBuffer);
}

void VertexArray_t::Bind() const
{
    glBindVertexArray(VertexArray);
}