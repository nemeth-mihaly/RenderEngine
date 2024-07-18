#pragma once

#include <stddef.h>
#include <stdint.h>
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
// class Shader
//-----------------------------------------------------------------------------

class Shader
{
 public:
    Shader();
    ~Shader();

    void Load(const std::string& vsResource, const std::string& fsResource);

    void Bind() const;

    void SetUniformBlockBinding(GLuint binding, const std::string& uniformBlockName);

    void SetUniform1b(const std::string& name, bool value);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1ui(const std::string& name, uint32_t value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, const glm::vec4& value);
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& value);

 private:
    GLint GetUniformLocation(const std::string& name);

 private:
    GLuint    m_programID;
};

typedef std::shared_ptr<Shader> StrongShaderPtr;