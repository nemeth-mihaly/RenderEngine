#pragma once

#include <cstddef>
#include <cstdint>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class VertexArray_t
{
 public:
    VertexArray_t(const void* InVerticesData, const uint32_t InNumVertices);
    ~VertexArray_t();

    void Bind() const;

    uint32_t GetNumVertices() const { return NumVertices; }

 private:
    uint32_t NumVertices;
    GLuint VertexBuffer;

    GLuint VertexArray;
};