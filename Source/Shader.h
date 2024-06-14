#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

//-----------------------------------------------------------------------------
// class _ShaderStage
//-----------------------------------------------------------------------------

class _ShaderStage
{
    friend class Shader;

    GLuint          m_shaderID;

public:
    _ShaderStage(GLenum type, const std::string& resource)
        : m_type(type), m_resource(resource)
    {
    }

    ~_ShaderStage()
    {
        glDeleteShader(m_shaderID);
    }

    void Load()
    {
        size_t size;
        char* pRawData = nullptr;
        LoadResource(m_resource, size, pRawData);

        m_shaderID = glCreateShader(m_type);
        
        glShaderSource(m_shaderID, 1, &pRawData, nullptr);
        delete[] pRawData;

        glCompileShader(m_shaderID);

        int bResult;
        glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &bResult);

        if (bResult != GL_TRUE)
        {
            const int infoLogSize = 512;
            char infoLog[infoLogSize];

            glGetShaderInfoLog(m_shaderID, infoLogSize, nullptr, infoLog);
            printf_s("%s\n", infoLog);
        }
    }

private:
    void LoadResource(const std::string& name, size_t& size, char*& pRawData)
    {
        std::ifstream file(name, std::ios::ate | std::ios::binary);

        size = static_cast<size_t>(file.tellg());
        file.seekg(0);

        pRawData = new char[size + 1];
        file.read(pRawData, size);
        pRawData[size] = '\0';

        file.close();
    }

private:
    GLenum          m_type;
    std::string     m_resource;
};

//-----------------------------------------------------------------------------
// class Shader
//-----------------------------------------------------------------------------

class Shader
{
public:
    Shader() = default;
    Shader(std::vector<_ShaderStage> stages);
    ~Shader();

    void LoadFromFile(const std::string& vertexShaderResource, const std::string& fragShaderResource);

    void Bind();

    void SetUniformBlockBinding(uint32_t binding, const std::string& uniformBlockName);

    // TODO: Perform type checks in the uniform setter functions.
    // Why though? Because for some reason, neither the c++ compiler nor the glsl compiler notifies 
    // if you pass the wrong type as parameter, resulting in bugs.

    // NOTE: The Uniform1b should be interpreted as an integer 
    // in the shader source because GLSL lacks a boolean type.
    void SetUniform1b(const std::string& name, bool value);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1ui(const std::string& name, uint32_t value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& value);

private:
    uint32_t GetUniformLocation(const std::string& name);

private:
    uint32_t    m_programID;
};

typedef std::shared_ptr<Shader> StrongShaderPtr;