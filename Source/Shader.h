#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

////////////////////////////////////////////////////
//  class VertexArray
////////////////////////////////////////////////////

class Shader
{
public:
    Shader();
    ~Shader();

    void LoadFromFile(const std::string& vertexShaderResource, const std::string& fragShaderResource);

    void Bind();

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
    uint32_t    m_ProgramID;
};

typedef std::shared_ptr<Shader> StrongShaderPtr;