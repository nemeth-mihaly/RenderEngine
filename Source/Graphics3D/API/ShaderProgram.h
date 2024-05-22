#pragma once

#include "Common.h"

#include <string>

////////////////////////////////////////////////////
//  class ShaderProgram
////////////////////////////////////////////////////

class ShaderProgram
{
    friend class ProgramPipeline;

 public:
    ShaderProgram();
    ~ShaderProgram();

    void Create(GLenum type);
    void SetSource(GLsizei count, const GLchar* const* string, const GLint* length) const;
    void Compile() const;

 private:
    GLuint      m_ShaderProgramID;
};

typedef std::shared_ptr<ShaderProgram> StrongShaderProgramPtr;