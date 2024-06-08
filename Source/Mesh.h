#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <memory>
#include <vector>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "VertexArray.h"

////////////////////////////////////////////////////
//  struct Vertex
////////////////////////////////////////////////////

struct Vertex
{
    glm::vec3     Pos;
    glm::vec3     Normal;
    glm::vec2     Uv;
};

////////////////////////////////////////////////////
//  class Mesh
////////////////////////////////////////////////////

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void LoadResource(const std::string& name);

    uint32_t GetVertexCount() const { return m_VertCount; }
    const StrongVertexArrayPtr& GetVertexArray() const { return m_VertexArray; }

private:
    uint32_t                m_VertCount;

    StrongVertexArrayPtr    m_VertexArray;
    StrongVertexBufferPtr   m_VertexBuffer;
    StrongIndexBufferPtr    m_IndexBuffer;
};

typedef std::shared_ptr<Mesh> StrongMeshPtr;