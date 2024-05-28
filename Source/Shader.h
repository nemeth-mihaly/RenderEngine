#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

class Shader_t
{
 public:
    Shader_t();
    ~Shader_t();

    void LoadFromFile(const std::string& VertexShaderFilename, const std::string& FragShaderFilename);

    void Bind() const;

    // TODO: Perform type checks in the uniform setter functions.
    // Why though? Because for some reason, neither the c++ compiler nor the glsl compiler notifies 
    // if you pass the wrong type as parameter, resulting in bugs.

    // NOTE: The Uniform1b should be interpreted as an integer 
    // in the shader source because GLSL lacks a boolean type.
    void SetUniform1b(const std::string& Name, bool Value) const;
    void SetUniform1i(const std::string& Name, int Value) const;
    void SetUniform1ui(const std::string& Name, uint32_t Value) const;
    void SetUniform1f(const std::string& Name, float Value) const;
    void SetUniform3f(const std::string& Name, const glm::vec3& Value) const;
    void SetUniform4f(const std::string& Name, const glm::vec4& Value) const;
    void SetUniformMatrix4f(const std::string& Name, const glm::mat4& Value) const;

 private:
    GLuint CompileShader(const std::string& Filename, GLenum ShaderType);
    GLint GetUniformLocation(const std::string& Name) const;

 private:
    GLuint m_ShaderProgram;
    GLuint m_VertexShader;
    GLuint m_FragShader;
};