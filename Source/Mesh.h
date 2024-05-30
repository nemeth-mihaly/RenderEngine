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
//  class Mesh_t
////////////////////////////////////////////////////

class Mesh_t
{
 public:
    Mesh_t();
    ~Mesh_t();

    void LoadFromFile(const std::string& filename);

    GLsizei GetVertexCount() { return m_VertexCount; }
    
    const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

 private:
    GLsizei m_VertexCount;
    std::shared_ptr<VertexArray> m_VertexArray;
};