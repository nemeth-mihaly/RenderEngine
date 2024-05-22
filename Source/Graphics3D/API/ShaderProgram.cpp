#include "Graphics3D/API/ShaderProgram.h"

#include <format>
#include <iostream>

////////////////////////////////////////////////////
//  ShaderProgram Implementation
////////////////////////////////////////////////////

ShaderProgram::ShaderProgram()
{
    m_ShaderProgramID = 0xFFFFFFFF;
}

ShaderProgram::~ShaderProgram()
{
    if (m_ShaderProgramID != 0xFFFFFFFF)
        glDeleteShader(m_ShaderProgramID);
}

void ShaderProgram::Create(GLenum type)
{
    m_ShaderProgramID = glCreateShader(type);
}

void ShaderProgram::SetSource(GLsizei count, const GLchar* const* string, const GLint* length) const
{
    glShaderSource(m_ShaderProgramID, count, string, length);
}

void ShaderProgram::Compile() const
{
    glCompileShader(m_ShaderProgramID);

    GLint result;
    glGetShaderiv(m_ShaderProgramID, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        const GLsizei n = 512;
        GLchar buf[n];

        glGetShaderInfoLog(m_ShaderProgramID, n, nullptr, buf);
        std::cout << std::format("{}\n", buf);
    }
}