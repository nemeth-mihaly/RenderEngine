#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>

#include "3rdParty/glm/glm.hpp"

#include "VertexArray.h"
#include "Buffers.h"

//-----------------------------------------------------------------------------
// class Mesh
//-----------------------------------------------------------------------------

class Mesh
{
public:
    Mesh();

    void Load(const std::string& filename);

    void Render();

    glm::vec3 GetBoundingBoxCenter() { return m_boundingBoxCenter; }     
    glm::vec3 GetBoundingBoxExtents() { return m_boundingBoxExtents; }

private:
    glm::vec3       m_boundingBoxCenter;
    glm::vec3       m_boundingBoxExtents;

    int             m_numVerts;
    VertexArray     m_vertexArray;
    VertexBuffer    m_vertexBuffer;
};