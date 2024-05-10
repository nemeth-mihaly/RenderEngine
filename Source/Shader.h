#pragma once

#include <string>

#include "GLCommon.h"

class Shader
{
 public:
    Shader(
        const std::string& name, 
        const std::string& vertShaderAssetName, 
        const std::string& fragShaderAssetName
    );
    ~Shader();

    void Init();

    void Use() const;

    // The Uniform1b should be interpreted as an integer in the shader source because GLSL lacks a boolean type.
    void Uniform1b(const std::string& name, bool value) const;
    void Uniform1i(const std::string& name, int value) const;
    void Uniform1ui(const std::string& name, uint32_t value) const;
    void UniformMatrix4f(const std::string& name, const glm::mat4& value) const;

 private:
    GLint GetUniformLocation(const std::string& name) const;

 private:
    std::string m_Name;
    std::string m_VertShaderAssetName;
    std::string m_FragShaderAssetName;
    GLuint m_ProgramID;
};