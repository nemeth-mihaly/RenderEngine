#include "Graphics3D/API/ProgramPipeline.h"

#include <format>
#include <iostream>

////////////////////////////////////////////////////
//  ProgramPipeline Implementation
////////////////////////////////////////////////////

ProgramPipeline::ProgramPipeline()
{
    m_ProgramPipelineID = 0xFFFFFFFF;
}

ProgramPipeline::~ProgramPipeline()
{
    if (m_ProgramPipelineID != 0xFFFFFFFF)
        glDeleteProgram(m_ProgramPipelineID);
}

void ProgramPipeline::Create()
{
    m_ProgramPipelineID = glCreateProgram();
}

void ProgramPipeline::SetActive(GLboolean bActive) const
{
    glUseProgram((bActive ? m_ProgramPipelineID : 0));
}

void ProgramPipeline::AttachShaderProgramStage(StrongShaderProgramPtr& shaderprogram) const
{
    glAttachShader(m_ProgramPipelineID, shaderprogram->m_ShaderProgramID);
}

void ProgramPipeline::Link() const
{
    glLinkProgram(m_ProgramPipelineID);

    GLint result;
    glGetProgramiv(m_ProgramPipelineID, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        const GLsizei n = 512;
        GLchar buf[n];

        glGetProgramInfoLog(m_ProgramPipelineID, n, nullptr, buf);
        std::cout << std::format("{}\n", buf);
    }
}

GLint ProgramPipeline::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_ProgramPipelineID, name.c_str());
}

void ProgramPipeline::SetUniform1b(const std::string& name, bool value) const
{
    SetUniform1i(name, static_cast<int>(value));
}

void ProgramPipeline::SetUniform1i(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void ProgramPipeline::SetUniform1ui(const std::string& name, uint32_t value) const
{
    glUniform1ui(GetUniformLocation(name), value);
}

void ProgramPipeline::SetUniform1f(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void ProgramPipeline::SetUniform3f(const std::string& name, const glm::vec3& value) const
{
    glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
}

void ProgramPipeline::SetUniform4f(const std::string& name, const glm::vec4& value) const
{
    glUniform4f(GetUniformLocation(name), value[0], value[1], value[2], value[3]);
}

void ProgramPipeline::SetUniformMatrix4f(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}