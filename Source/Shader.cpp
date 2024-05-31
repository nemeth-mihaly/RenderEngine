#include "Shader.h"

Shader_t::Shader_t()
{
}

Shader_t::~Shader_t()
{
}

void Shader_t::LoadFromFile(const std::string& VertexShaderFilename, const std::string& FragShaderFilename)
{
    m_VertexShader = CompileShader(VertexShaderFilename, GL_VERTEX_SHADER);
    m_FragShader = CompileShader(FragShaderFilename, GL_FRAGMENT_SHADER);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, m_VertexShader);
    glAttachShader(m_ShaderProgram, m_FragShader);
    glLinkProgram(m_ShaderProgram);
}

void Shader_t::Bind() const
{
    glUseProgram(m_ShaderProgram);
}

void Shader_t::SetUniform1b(const std::string& Name, bool Value) const
{
    SetUniform1i(Name, static_cast<int>(Value));
}

void Shader_t::SetUniform1i(const std::string& Name, int Value) const
{
    glUniform1i(GetUniformLocation(Name), Value);
}

void Shader_t::SetUniform1ui(const std::string& Name, uint32_t Value) const
{
    glUniform1ui(GetUniformLocation(Name), Value);
}

void Shader_t::SetUniform1f(const std::string& Name, float Value) const
{
    glUniform1f(GetUniformLocation(Name), Value);
}

void Shader_t::SetUniform3f(const std::string& Name, const glm::vec3& Value) const
{
    glUniform3f(GetUniformLocation(Name), Value[0], Value[1], Value[2]);
}

void Shader_t::SetUniform4f(const std::string& Name, const glm::vec4& Value) const
{
    glUniform4f(GetUniformLocation(Name), Value[0], Value[1], Value[2], Value[3]);
}

void Shader_t::SetUniformMatrix4f(const std::string& Name, const glm::mat4& Value) const
{
    glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, glm::value_ptr(Value));
}

GLuint Shader_t::CompileShader(const std::string& Filename, GLenum ShaderType)
{
    GLuint shaderID = glCreateShader(ShaderType);

    FILE* File = fopen(Filename.c_str(), "rb");

    fseek(File, 0, SEEK_END);
    size_t FileSize = ftell(File);
    rewind(File);

    char* Buf = new char[FileSize + 1];
    fread(&Buf[0], sizeof(char), FileSize, File);
    Buf[FileSize] = '\0';

    fclose(File);

    glShaderSource(shaderID, 1, &Buf, NULL);
    glCompileShader(shaderID);

    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        char infoLog[BUFSIZ];
        glGetShaderInfoLog(shaderID, BUFSIZ, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    delete[] Buf;

    return shaderID;
}

GLint Shader_t::GetUniformLocation(const std::string& Name) const
{
    return glGetUniformLocation(m_ShaderProgram, Name.c_str());
}