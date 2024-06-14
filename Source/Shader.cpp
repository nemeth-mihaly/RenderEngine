#include "Shader.h"

//-----------------------------------------------------------------------------
// Shader Implementation
//-----------------------------------------------------------------------------

Shader::Shader(std::vector<_ShaderStage> stages)
{
    m_programID = glCreateProgram();

    for (_ShaderStage& stage : stages)
    {
        stage.Load();
        glAttachShader(m_programID, stage.m_shaderID);
    }

    glLinkProgram(m_programID);

    int bResult;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &bResult);

    if (bResult != GL_TRUE)
    {
        const int infoLogSize = 512;
        char infoLog[infoLogSize];

        glGetProgramInfoLog(m_programID, infoLogSize, nullptr, infoLog);
        printf_s("%s\n", infoLog);
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

void Shader::LoadFromFile(const std::string& vertexShaderResource, const std::string& fragShaderResource)
{
    auto Compile = [](const std::string& resource, uint32_t shaderType)
    {
        uint32_t shaderID = glCreateShader(shaderType);

        FILE* fp = fopen(resource.c_str(), "rb");

        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        rewind(fp);

        char* pData = new char[size + 1];
        fread(&pData[0], sizeof(char), size, fp);
        pData[size] = '\0';

        fclose(fp);

        glShaderSource(shaderID, 1, &pData, NULL);
        glCompileShader(shaderID);

        int result;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            char infoLog[BUFSIZ];
            glGetShaderInfoLog(shaderID, BUFSIZ, NULL, infoLog);
            printf("%s\n", infoLog);
        }

        delete[] pData;

        return shaderID;
    };

    uint32_t vertexShaderID = Compile(vertexShaderResource, GL_VERTEX_SHADER);
    uint32_t fragShaderID = Compile(fragShaderResource, GL_FRAGMENT_SHADER);

    m_programID = glCreateProgram();

    glAttachShader(m_programID, vertexShaderID);
    glAttachShader(m_programID, fragShaderID);

    glLinkProgram(m_programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragShaderID);
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