#include "Texture.h"

//-----------------------------------------------------------------------------
// Texture Implementation
//-----------------------------------------------------------------------------

Texture::Texture(uint32_t target, uint32_t wrapping, uint32_t filtering)
{
    glCreateTextures(target, 1, &m_textureID);

    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, wrapping);
    glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, wrapping);
    glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, filtering);
    glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, filtering);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureID);
}

void Texture::Load(const std::string& resource)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channelCount;
    
    uint8_t* pData = stbi_load(resource.c_str(), &width, &height, &channelCount, 0);

    if (!pData)
    {
        return;
    }

    uint32_t internalFormat, format;

    switch (channelCount)
    {
        case 3:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            break;

        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;

        default:
            assert(true);
    }

    glTextureStorage2D(m_textureID, 1, internalFormat, width, height);
    glTextureSubImage2D(m_textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pData);

    stbi_image_free(pData);
}

void Texture::BindUnit(uint32_t unit)
{
    glBindTextureUnit(unit, m_textureID);
}