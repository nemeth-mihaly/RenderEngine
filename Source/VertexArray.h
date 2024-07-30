#pragma once

#include <stddef.h>
#include <stdint.h>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"

#include "Buffers.h"

//-----------------------------------------------------------------------------
// struct Vertex_UnlitColored
//-----------------------------------------------------------------------------

struct Vertex_UnlitColored
{
    glm::vec3   pos;
    glm::vec3   color;
};

//-----------------------------------------------------------------------------
// struct Vertex_UnlitTextured
//-----------------------------------------------------------------------------

struct Vertex_UnlitTextured
{
    glm::vec3   pos;
    glm::vec2   uv;
};

//-----------------------------------------------------------------------------
// struct Vertex_UnlitTexturedColored
//-----------------------------------------------------------------------------

struct Vertex_UnlitTexturedColored
{
    glm::vec3   pos;
    glm::vec3   color;
    glm::vec2   uv;
};

//-----------------------------------------------------------------------------
// struct Vertex_LitTexturedColored
//-----------------------------------------------------------------------------

struct Vertex_LitTexturedColored
{
    glm::vec3   pos;
    glm::vec3   norm;
    glm::vec3   color;
    glm::vec2   uv;
};

//-----------------------------------------------------------------------------
// enum VertexArrayInputRate
//-----------------------------------------------------------------------------

enum VertexArrayInputRate
{
    VertexArrayInputRate_Vertex      = 0,
    VertexArrayInputRate_Instance    = 1
};

//-----------------------------------------------------------------------------
// class VertexArray
//-----------------------------------------------------------------------------

class VertexArray
{
 public:
    VertexArray();
    ~VertexArray();

    void Init();

    void Bind() const;

    void SetVertexBuffer(GLuint binding, VertexBuffer* pVertexBuffer, GLsizei stride, GLuint inputRate);
    void SetIndexBuffer(IndexBuffer* pIndexBuffer);

    void SetVertexAttribute(GLuint binding, GLuint location, GLint size, GLenum type, GLuint offset);

 public:
   GLuint   m_arrayID;
};