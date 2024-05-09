#pragma once

#include <string>

#include "glad/glad_wgl.h"
#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

constexpr GLuint InvalidGpuID = 0xffffffff;

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

    void UniformMatrix4f(const std::string& name, const glm::mat4& matrix) const;

 private:
    GLint GetUniformLocation(const std::string& name) const;

 private:
    std::string m_Name;
    std::string m_VertShaderAssetName;
    std::string m_FragShaderAssetName;
    GLuint m_ProgramID;
};