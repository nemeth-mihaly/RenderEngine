#include "Shader.h"

#include <fstream>

char* LoadShaderResource(const std::string& name)
{
    std::ifstream file(name, std::ios::binary);

    file.seekg(0, file.end);
    const size_t size = static_cast<size_t>(file.tellg());
    file.seekg(0, file.beg);

    char* pBuffer = new char[size + 1];
    file.read(pBuffer, size);
    pBuffer[size] = '\0';

    file.close();

    return pBuffer;
}

//-----------------------------------------------------------------------------
// Shader Implementation
//-----------------------------------------------------------------------------

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::Load(const std::string& vertResource, const std::string& fragResource)
{
    char* pVertResBuffer = LoadShaderResource(vertResource);
    char* pFragResBuffer = LoadShaderResource(fragResource);

    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertID, 1, &pVertResBuffer, nullptr);
    glShaderSource(fragID, 1, &pFragResBuffer, nullptr);

    delete[] pVertResBuffer;
    delete[] pFragResBuffer;

    glCompileShader(vertID);
    glCompileShader(fragID);

    m_programID = glCreateProgram();

    glAttachShader(m_programID, vertID);
    glAttachShader(m_programID, fragID);    

    glLinkProgram(m_programID);
}

void Shader::Bind()
{
    glUseProgram(m_programID);
}

void Shader::SetUniformBlockBinding(uint32_t binding, const std::string& uniformBlockName)
{
    const uint32_t uniformBlockIndex = glGetUniformBlockIndex(m_programID, uniformBlockName.c_str());
    glUniformBlockBinding(m_programID, uniformBlockIndex, binding);
}

void Shader::SetUniform1b(const std::string& name, bool value)
{
    SetUniform1i(name, static_cast<int>(value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1ui(const std::string& name, uint32_t value)
{
    glUniform1ui(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
    glUniform4f(GetUniformLocation(name), value[0], value[1], value[2], value[3]);
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

uint32_t Shader::GetUniformLocation(const std::string& name)
{
    return glGetUniformLocation(m_programID, name.c_str());
}