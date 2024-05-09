#pragma once

#include <string>

#include "GLCommon.h"
//#include "glad/glad_wgl.h"
//#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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