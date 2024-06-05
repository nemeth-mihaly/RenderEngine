#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

////////////////////////////////////////////////////
//  class UniformBuffer
////////////////////////////////////////////////////

class UniformBuffer
{
public:
    UniformBuffer(uint32_t binding, ssize_t size, uint32_t usage);
    ~UniformBuffer();

    void MapMemory(int64_t offset, ssize_t size, const void* pData);

private:
    ssize_t     m_Size;
    uint32_t    m_BufferID;
};

typedef std::shared_ptr<UniformBuffer> StrongUniformBufferPtr;

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
//  class IndexBuffer
////////////////////////////////////////////////////

class IndexBuffer
{
    friend class VertexArray;

public:
    IndexBuffer(ssize_t size, uint32_t usage);
    ~IndexBuffer();

    void MapMemory(int64_t offset, ssize_t size, const void* pData);

private:
    ssize_t     m_Size;
    uint32_t    m_BufferID;
};

typedef std::shared_ptr<IndexBuffer> StrongIndexBufferPtr;