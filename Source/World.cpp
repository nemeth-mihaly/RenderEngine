#include "World.h"

#include <fstream>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// World Implementation
//-----------------------------------------------------------------------------

World::World()
{
    m_NumVerts = 0;
}

World::~World()
{
    glDeleteBuffers(1, &m_VboId);
    glDeleteVertexArrays(1, &m_VaoId);
}

void World::Init()
{
    uint32_t vsId, fsId;
    m_ShaderId = glCreateProgram();

    {
        std::ifstream file("Assets/Shaders/World.vert", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        vsId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsId, 1, &pSource, nullptr);
        glCompileShader(vsId);

        delete[] pSource;
    }

    {
        std::ifstream file("Assets/Shaders/World.frag", std::ios::binary | std::ios::ate);
        size_t size = (size_t)file.tellg();
        file.seekg(0);

        char* pSource = new char[size + 1];
        file.read(pSource, size);
        pSource[size] = '\0';
        file.close();

        fsId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsId, 1, &pSource, nullptr);
        glCompileShader(fsId);

        delete[] pSource;
    }

    glAttachShader(m_ShaderId, vsId);
    glAttachShader(m_ShaderId, fsId);
    glLinkProgram(m_ShaderId);

    glDeleteShader(vsId);
    glDeleteShader(fsId);

    glUniformBlockBinding(m_ShaderId, glGetUniformBlockIndex(m_ShaderId, "Matrices"), 0);

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

void World::Update(float deltaTime)
{

}

void World::Draw()
{
    glUseProgram(m_ShaderId);

    glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, "uModel"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));  

    glBindVertexArray(m_VaoId);
    glDrawArrays(GL_TRIANGLES, 0, m_NumVerts);

    glBindVertexArray(0);
    glUseProgram(0);
}