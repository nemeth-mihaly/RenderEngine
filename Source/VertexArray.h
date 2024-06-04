#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

////////////////////////////////////////////////////
//  class VertexBuffer
////////////////////////////////////////////////////

class VertexBuffer
{
    friend class VertexArray;

public:
    VertexBuffer(ssize_t size, uint32_t usage);
    ~VertexBuffer();

    void MapMemory(int64_t offset, ssize_t size, const void* pData);

private:
    ssize_t     m_Size;
    uint32_t    m_BufferID;
};

typedef std::shared_ptr<VertexBuffer> StrongVertexBufferPtr;

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
    void SetVertexAttribute(uint32_t binding, uint32_t location, int size, uint32_t type, uint32_t offset);

public:
   uint32_t   m_ArrayID;
};

typedef std::shared_ptr<VertexArray> StrongVertexArrayPtr;