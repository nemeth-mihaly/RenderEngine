#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "Buffers.h"

////////////////////////////////////////////////////
//  enum VertexArrayInputRate
////////////////////////////////////////////////////

enum VertexArrayInputRate
{
    VertexArrayInputRate_Vertex      = 0,
    VertexArrayInputRate_Instance    = 1
};

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;

    void SetVertexBuffer(uint32_t binding, StrongVertexBufferPtr& vertexBuffer, int stride, uint32_t inputRate);
    void SetIndexBuffer(StrongIndexBufferPtr& indexBuffer);

    void SetVertexAttribute(uint32_t binding, uint32_t location, int size, uint32_t type, uint32_t offset);

public:
   uint32_t   m_ArrayID;
};

typedef std::shared_ptr<VertexArray> StrongVertexArrayPtr;