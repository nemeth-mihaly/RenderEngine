#include "Texture.h"

#include <cassert>

#include "stb/stb_image.h"

Texture::Texture(const std::string& name, const std::string& textureAssetName)
    : m_Name(name), m_TextureAssetName(textureAssetName)
{
    m_TextureID = InvalidGpuID;
}

Texture::~Texture()
{
    if (m_TextureID != InvalidGpuID)
    {
        glDeleteTextures(1, &m_TextureID);
    }
}

void Texture::Init()
{
    // NOTE: Probably not the optimal location for reading assets.
    uint8_t* pPixels = stbi_load(m_TextureAssetName.c_str(), &m_Width, &m_Height, &m_ChannelCount, 0);
    assert(pPixels);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum internalFormat, format;
    
    if (m_ChannelCount == 3)
    {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    }
    else
    if (m_ChannelCount == 4)
    {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }
    else
    {
        assert(1);
    }

    glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);
    glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, pPixels);

    stbi_image_free(pPixels);
}

void Texture::Bind(GLuint unit) const
{
    glBindTextureUnit(unit, m_TextureID);
}