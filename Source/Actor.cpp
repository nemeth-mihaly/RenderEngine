#include "Actor.h"

#include <cstdio>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// Actor Implementation
//-----------------------------------------------------------------------------

Actor::Actor()
{
    printf("Creating Actor\n");

    m_Pos = glm::vec3(0.0f, 0.0f, 0.0f);

    m_NumVerts = 0;
}

Actor::~Actor()
{
    printf("Destroying Actor\n");

    glDeleteBuffers(1, &m_VboId);
    glDeleteVertexArrays(1, &m_VaoId);
}

void Actor::Init()
{
    struct Vertex
    {
        glm::vec3   Pos;
        glm::vec3   Color;
    };

    std::vector<Vertex> vertices;
    vertices.resize(3);

    vertices[0].Pos = glm::vec3(-0.5f,-0.5f, 0.0f); vertices[0].Color = glm::vec3(1.0f, 0.0f, 0.0f);
    vertices[1].Pos = glm::vec3( 0.5f,-0.5f, 0.0f); vertices[1].Color = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].Pos = glm::vec3( 0.0f, 0.5f, 0.0f); vertices[2].Color = glm::vec3(0.0f, 0.0f, 1.0f);

    glCreateVertexArrays(1, &m_VaoId);

    glCreateBuffers(1, &m_VboId);	
    glNamedBufferData(m_VboId, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);;

    glVertexArrayVertexBuffer(m_VaoId, 0, m_VboId, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(m_VaoId, 0);
    glEnableVertexArrayAttrib(m_VaoId, 1);

    glVertexArrayAttribFormat(m_VaoId, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VaoId, 1, 3, GL_FLOAT, GL_FALSE, 12);

    glVertexArrayAttribBinding(m_VaoId, 0, 0);
    glVertexArrayAttribBinding(m_VaoId, 1, 0);

    m_NumVerts = vertices.size();
    vertices.clear();
}

void Actor::Draw()
{
    glBindVertexArray(m_VaoId);
    glDrawArrays(GL_TRIANGLES, 0, m_NumVerts);

    glBindVertexArray(0);
}