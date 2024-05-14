#include "Graphics3D/ShaderProgram.h"

#include <cstdio>

////////////////////////////////////////////////////
//  Shader Implementation
////////////////////////////////////////////////////

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteShader(m_ShaderID);
}

void Shader::Create(GLenum type)
{
    m_ShaderID = glCreateShader(type);
}

void Shader::SetSource(GLsizei count, const GLchar* const* string, const GLint* length) const
{
    glShaderSource(m_ShaderID, count, string, length);
}

void Shader::Compile() const
{
    glCompileShader(m_ShaderID);

    GLint result;
    glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        const GLsizei n = 512;
        GLchar log[n];

        glGetShaderInfoLog(m_ShaderID, n, nullptr, log);

        fprintf(stderr, "%s \n", log);
    }
}

////////////////////////////////////////////////////
//  ShaderProgram Implementation
////////////////////////////////////////////////////

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_ProgramID);
}

void ShaderProgram::Create()
{
    m_ProgramID = glCreateProgram();
}

void ShaderProgram::AttachShader(const StrongShaderPtr& shader) const
{
    glAttachShader(m_ProgramID, shader->m_ShaderID);
}

void ShaderProgram::Link() const
{
    glLinkProgram(m_ProgramID);

    GLint result;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        const GLsizei n = 512;
        GLchar log[n];

        glGetProgramInfoLog(m_ProgramID, n, nullptr, log);

        fprintf(stderr, "%s \n", log);
    }
}

void ShaderProgram::Use() const
{
    glUseProgram(m_ProgramID);
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_ProgramID, name.c_str());
}

void ShaderProgram::SetUniform1b(const std::string& name, bool value) const
{
    SetUniform1i(name, static_cast<int>(value));
}

void ShaderProgram::SetUniform1i(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform1ui(const std::string& name, uint32_t value) const
{
    glUniform1ui(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform1f(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& value) const
{
    glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
}

void ShaderProgram::SetUniformMatrix4f(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}