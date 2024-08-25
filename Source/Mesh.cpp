#include "Mesh.h"

#include <cstdio>
#include <cassert>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// Mesh Implementation
//-----------------------------------------------------------------------------

Mesh::Mesh()
{
    m_NumVerts = 0;
}

Mesh::~Mesh()
{
    glDeleteTextures(1, &m_TextureId);

    glDeleteBuffers(1, &m_VboId);
    glDeleteVertexArrays(1, &m_VaoId);
}

void Mesh::Init()
{
    struct Vertex
    {
        glm::vec3   Pos;
        glm::vec3   Color;
        glm::vec2   Uv;
    };

    std::vector<Vertex> vertices;
    vertices.resize(36);

    // Front face
    vertices[0].Pos = glm::vec3(-0.5f, 0.5f, 0.5f); vertices[0].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[0].Uv = glm::vec2(0.0f, 1.0f);
    vertices[1].Pos = glm::vec3(-0.5f,-0.5f, 0.5f); vertices[1].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[1].Uv = glm::vec2(0.0f, 0.0f);
    vertices[2].Pos = glm::vec3( 0.5f, 0.5f, 0.5f); vertices[2].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[2].Uv = glm::vec2(1.0f, 1.0f);

    vertices[3].Pos = glm::vec3( 0.5f, 0.5f, 0.5f); vertices[3].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[3].Uv = glm::vec2(1.0f, 1.0f);
    vertices[4].Pos = glm::vec3(-0.5f,-0.5f, 0.5f); vertices[4].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[4].Uv = glm::vec2(0.0f, 0.0f);
    vertices[5].Pos = glm::vec3( 0.5f,-0.5f, 0.5f); vertices[5].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[5].Uv = glm::vec2(1.0f, 0.0f);

    // Back face
    vertices[6].Pos = glm::vec3(-0.5f, 0.5f,-0.5f); vertices[6].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[6].Uv = glm::vec2(0.0f, 1.0f);
    vertices[7].Pos = glm::vec3(-0.5f,-0.5f,-0.5f); vertices[7].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[7].Uv = glm::vec2(0.0f, 0.0f);
    vertices[8].Pos = glm::vec3( 0.5f, 0.5f,-0.5f); vertices[8].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[8].Uv = glm::vec2(1.0f, 1.0f);

    vertices[9].Pos = glm::vec3( 0.5f, 0.5f,-0.5f); vertices[9].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[9].Uv = glm::vec2(1.0f, 1.0f);
    vertices[10].Pos = glm::vec3(-0.5f,-0.5f,-0.5f); vertices[10].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[10].Uv = glm::vec2(0.0f, 0.0f);
    vertices[11].Pos = glm::vec3( 0.5f,-0.5f,-0.5f); vertices[11].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[11].Uv = glm::vec2(1.0f, 0.0f);

    // Top face
    vertices[12].Pos = glm::vec3(-0.5f, 0.5f,-0.5f); vertices[12].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[12].Uv = glm::vec2(0.0f, 1.0f);
    vertices[13].Pos = glm::vec3(-0.5f, 0.5f, 0.5f); vertices[13].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[13].Uv = glm::vec2(0.0f, 0.0f);
    vertices[14].Pos = glm::vec3( 0.5f, 0.5f,-0.5f); vertices[14].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[14].Uv = glm::vec2(1.0f, 1.0f);

    vertices[15].Pos = glm::vec3( 0.5f, 0.5f,-0.5f); vertices[15].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[15].Uv = glm::vec2(1.0f, 1.0f);
    vertices[16].Pos = glm::vec3(-0.5f, 0.5f, 0.5f); vertices[16].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[16].Uv = glm::vec2(0.0f, 0.0f);
    vertices[17].Pos = glm::vec3( 0.5f, 0.5f, 0.5f); vertices[17].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[17].Uv = glm::vec2(1.0f, 0.0f);

    // Bottom face
    vertices[18].Pos = glm::vec3(-0.5f,-0.5f,-0.5f); vertices[18].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[18].Uv = glm::vec2(0.0f, 1.0f);
    vertices[19].Pos = glm::vec3(-0.5f,-0.5f, 0.5f); vertices[19].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[19].Uv = glm::vec2(0.0f, 0.0f);
    vertices[20].Pos = glm::vec3( 0.5f,-0.5f,-0.5f); vertices[20].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[20].Uv = glm::vec2(1.0f, 1.0f);

    vertices[21].Pos = glm::vec3( 0.5f,-0.5f,-0.5f); vertices[21].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[21].Uv = glm::vec2(1.0f, 1.0f);
    vertices[22].Pos = glm::vec3(-0.5f,-0.5f, 0.5f); vertices[22].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[22].Uv = glm::vec2(0.0f, 0.0f);
    vertices[23].Pos = glm::vec3( 0.5f,-0.5f, 0.5f); vertices[23].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[23].Uv = glm::vec2(1.0f, 0.0f);

    // Right face
    vertices[24].Pos = glm::vec3( 0.5f, 0.5f,-0.5f); vertices[24].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[24].Uv = glm::vec2(0.0f, 1.0f);
    vertices[25].Pos = glm::vec3( 0.5f,-0.5f,-0.5f); vertices[25].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[25].Uv = glm::vec2(0.0f, 0.0f);
    vertices[26].Pos = glm::vec3( 0.5f, 0.5f, 0.5f); vertices[26].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[26].Uv = glm::vec2(1.0f, 1.0f);

    vertices[27].Pos = glm::vec3( 0.5f, 0.5f, 0.5f); vertices[27].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[27].Uv = glm::vec2(1.0f, 1.0f);
    vertices[28].Pos = glm::vec3( 0.5f,-0.5f,-0.5f); vertices[28].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[28].Uv = glm::vec2(0.0f, 0.0f);
    vertices[29].Pos = glm::vec3( 0.5f,-0.5f, 0.5f); vertices[29].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[29].Uv = glm::vec2(1.0f, 0.0f);

    // Right face
    vertices[30].Pos = glm::vec3(-0.5f, 0.5f,-0.5f); vertices[30].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[30].Uv = glm::vec2(0.0f, 1.0f);
    vertices[31].Pos = glm::vec3(-0.5f,-0.5f,-0.5f); vertices[31].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[31].Uv = glm::vec2(0.0f, 0.0f);
    vertices[32].Pos = glm::vec3(-0.5f, 0.5f, 0.5f); vertices[32].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[32].Uv = glm::vec2(1.0f, 1.0f);

    vertices[33].Pos = glm::vec3(-0.5f, 0.5f, 0.5f); vertices[33].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[33].Uv = glm::vec2(1.0f, 1.0f);
    vertices[34].Pos = glm::vec3(-0.5f,-0.5f,-0.5f); vertices[34].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[34].Uv = glm::vec2(0.0f, 0.0f);
    vertices[35].Pos = glm::vec3(-0.5f,-0.5f, 0.5f); vertices[35].Color = glm::vec3(1.0f, 1.0f, 1.0f); vertices[35].Uv = glm::vec2(1.0f, 0.0f);

    glCreateVertexArrays(1, &m_VaoId);

    glCreateBuffers(1, &m_VboId);	
    glNamedBufferData(m_VboId, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);;

    glVertexArrayVertexBuffer(m_VaoId, 0, m_VboId, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(m_VaoId, 0);
    glEnableVertexArrayAttrib(m_VaoId, 1);
    glEnableVertexArrayAttrib(m_VaoId, 2);

    glVertexArrayAttribFormat(m_VaoId, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VaoId, 1, 3, GL_FLOAT, GL_FALSE, 12);
    glVertexArrayAttribFormat(m_VaoId, 2, 2, GL_FLOAT, GL_FALSE, 24);

    glVertexArrayAttribBinding(m_VaoId, 0, 0);
    glVertexArrayAttribBinding(m_VaoId, 1, 0);
    glVertexArrayAttribBinding(m_VaoId, 2, 0);

    m_NumVerts = vertices.size();
    vertices.clear();
}

void Mesh::Draw()
{
    glBindVertexArray(m_VaoId);
    glDrawArrays(GL_TRIANGLES, 0, m_NumVerts);

    glBindVertexArray(0);
}