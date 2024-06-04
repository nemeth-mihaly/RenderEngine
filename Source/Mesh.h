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

    void LoadFromFile(const std::string& resource);

    int GetVertexCount() { return m_VertexCount; }
    const StrongVertexArrayPtr& GetVertexArray() const { return m_VertexArray; }

private:
    int                     m_VertexCount;
    StrongVertexBufferPtr   m_VertexBuffer;
    StrongVertexArrayPtr    m_VertexArray;
};

typedef std::shared_ptr<Mesh> StrongMeshPtr;