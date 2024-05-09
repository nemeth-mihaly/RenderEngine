#pragma once

#include <string>

#include "GLCommon.h"

class Texture
{
 public:
    Texture(const std::string& name, const std::string& textureAssetName);
    ~Texture();

    void Init();

    void Bind(GLuint unit) const;

 private:
    std::string m_Name;
    std::string m_TextureAssetName;
    int m_Width;
    int m_Height;
    int m_ChannelCount;
    GLuint m_TextureID;
};