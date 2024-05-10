#include "Shader.h"

#include <cstdio>
#include <cassert>
#include <vector>

static char* _ReadFile(const std::string& name)
{
    FILE* fp = fopen(name.c_str(), "rb");
    assert(fp != nullptr);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* buf = new char[size + 1];
    fread(buf, sizeof(char), size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}

Shader::Shader(
    const std::string& name, 
    const std::string& vertShaderAssetName, 
    const std::string& fragShaderAssetName
)
    : m_Name(name), 
    m_VertShaderAssetName(vertShaderAssetName), 
    m_FragShaderAssetName(fragShaderAssetName)
{
    m_ProgramID = InvalidGpuID;
}

Shader::~Shader()
{
    if (m_ProgramID != InvalidGpuID) 
    {
        glDeleteProgram(m_ProgramID);
    }
}

void Shader::Init()
{
    std::vector<GLuint> shaderStageIDs;

    // NOTE: Probably not the optimal location for reading assets.
    const char* pVertShaderSource = _ReadFile(m_VertShaderAssetName);
    const char* pFragShaderSource = _ReadFile(m_FragShaderAssetName);

    auto CreateShaderStage = [&shaderStageIDs](GLenum stageType, const char* pSource)
    {
        GLuint shaderStageID = InvalidGpuID;
        shaderStageID = glCreateShader(stageType);
        assert(shaderStageID != InvalidGpuID);

        glShaderSource(shaderStageID, 1, &pSource, nullptr);
        glCompileShader(shaderStageID);

        GLint result;
        glGetShaderiv(shaderStageID, GL_COMPILE_STATUS, &result);

        if (result != GL_TRUE)
        {
            GLchar infoLog[BUFSIZ];
            glGetShaderInfoLog(shaderStageID, BUFSIZ, nullptr, infoLog);
            printf("%s \n", infoLog);
        }

        shaderStageIDs.push_back(shaderStageID);
    };

    // Vert shader ID
    CreateShaderStage(GL_VERTEX_SHADER, pVertShaderSource);

    // Frag shader ID
    CreateShaderStage(GL_FRAGMENT_SHADER, pFragShaderSource);

    m_ProgramID = glCreateProgram();
    assert(m_ProgramID != InvalidGpuID);

    for (uint32_t i = 0; i < shaderStageIDs.size(); ++i)
    {
        glAttachShader(m_ProgramID, shaderStageIDs[i]);
    }

    glLinkProgram(m_ProgramID);

    GLint result;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLchar infoLog[BUFSIZ];
        glGetProgramInfoLog(m_ProgramID, BUFSIZ, nullptr, infoLog);
        printf("%s \n", infoLog);
    }

    for (uint32_t i = 0; i < shaderStageIDs.size(); ++i)
    {
        glDeleteShader(shaderStageIDs[i]);
    }

    delete pFragShaderSource;
    delete pVertShaderSource;
}

void Shader::Use() const
{
    glUseProgram(m_ProgramID);
}

void Shader::Uniform1b(const std::string& name, bool value) const
{
    Uniform1i(name, static_cast<int>(value));
}

void Shader::Uniform1i(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::Uniform1ui(const std::string& name, uint32_t value) const
{
    glUniform1ui(GetUniformLocation(name), value);
}

void Shader::UniformMatrix4f(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_ProgramID, name.c_str());
}