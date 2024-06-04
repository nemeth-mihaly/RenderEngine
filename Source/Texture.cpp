#include "Texture.h"

////////////////////////////////////////////////////
//  Texture Implementation
////////////////////////////////////////////////////

Texture::Texture(uint32_t target)
{
    glCreateTextures(target, 1, &m_TextureID);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureID);
}

void Texture::LoadFromFile(const std::string& resource)
{
    stbi_set_flip_vertically_on_load(true);

    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channelCount;
    
    uint8_t* pData = stbi_load(resource.c_str(), &width, &height, &channelCount, 0);
    assert(pData != nullptr);

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

    glTextureStorage2D(m_TextureID, 1, internalFormat, width, height);
    glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pData);

    stbi_image_free(pData);
}

void Texture::BindUnit(uint32_t unit)
{
    glBindTextureUnit(unit, m_TextureID);
}