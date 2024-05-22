#pragma once

#include "Common.h"

#include <string>

#include "Graphics3D/API/ShaderProgram.h"

////////////////////////////////////////////////////
//  class ProgramPipeline
////////////////////////////////////////////////////

class ProgramPipeline
{
 public:
    ProgramPipeline();
    ~ProgramPipeline();

    void Create();
    void SetActive(GLboolean bActive = GL_TRUE) const;
    void AttachShaderProgramStage(StrongShaderProgramPtr& shaderprogram) const;
    void Link() const;

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
    void SetUniform4f(const std::string& name, const glm::vec4& value) const;
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& value) const;

 private:
    GLint GetUniformLocation(const std::string& name) const;

 private:
    GLuint      m_ProgramPipelineID;
};

typedef std::shared_ptr<ProgramPipeline> StrongProgramPipelinePtr;