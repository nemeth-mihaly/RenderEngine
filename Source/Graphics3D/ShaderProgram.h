#pragma once

#include "Common.h"

#include <string>

////////////////////////////////////////////////////
//  class Shader
////////////////////////////////////////////////////

class Shader
{
    friend class ShaderProgram;

 public:
    Shader();
    ~Shader();

    void Create(GLenum type);
    void SetSource(GLsizei count, const GLchar* const* string, const GLint* length) const;
    void Compile() const;

 private:
    GLuint m_ShaderID;
};

typedef std::shared_ptr<Shader> StrongShaderPtr;

////////////////////////////////////////////////////
//  class ShaderProgram
////////////////////////////////////////////////////

class ShaderProgram
{
 public:
    ShaderProgram();
    ~ShaderProgram();

    void Create();
    void AttachShader(const StrongShaderPtr& shader) const;
    void Link() const;
    void Use() const;

    // TODO: Perform type checks in the uniform setter functions.
    // Why though? Because for some reason, neither the c++ compiler nor the glsl compiler notifies 
    // if you pass the wrong type as parameter, resulting in bugs.

    // NOTE: The Uniform1b should be interpreted as an integer 
    // in the shader source because GLSL lacks a boolean type.
    void SetUniform1b(const std::string& name, bool value) const;
    
    void SetUniform1i(const std::string& name, int value) const;
    void SetUniform1ui(const std::string& name, uint32_t value) const;
    void SetUniform1f(const std::string& name, float value) const;
    void SetUniform3f(const std::string& name, const glm::vec3& value) const;
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& value) const;

 private:
    GLint GetUniformLocation(const std::string& name) const;

 private:
    GLuint m_ProgramID;
};

typedef std::shared_ptr<ShaderProgram> StrongShaderProgPtr;