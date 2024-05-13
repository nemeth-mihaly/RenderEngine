#include "Graphics3D/Texture.h"

GLenum InternalFormat(int channelCount)
{
    int internalformat;

    switch (channelCount)
    {
        case 3:
        {
            internalformat = GL_RGB8;
            break;
        }

        case 4:
        {
            internalformat = GL_RGBA8;
            break;
        }

        default:
            assert(true);
    }

    return internalformat;
}

GLenum Format(int channelCount)
{
    int format;

    switch (channelCount)
    {
        case 3:
        {
            format = GL_RGB;
            break;
        }

        case 4:
        {
            format = GL_RGBA;
            break;
        }

        default:
            assert(true);
    }

    return format;
}

////////////////////////////////////////////////////
//  Texture Implementation
////////////////////////////////////////////////////

Texture::Texture()
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureID);
}

void Texture::Create(GLenum target)
{
    glCreateTextures(target, 1, &m_TextureID);
}

void Texture::SetParamateri(GLenum pname, GLint param) const
{
    glTextureParameteri(m_TextureID, pname, param);
}

void Texture::SetStorage2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) const
{
    glTextureStorage2D(m_TextureID, levels, internalformat, width, height);
}

void Texture::SetSubImage2D(
    GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, 
    GLenum format, GLenum type, const void *pPixels
) const
{
    glTextureSubImage2D(m_TextureID, level, xoffset, yoffset, width, height, format, type, pPixels);
}

void Texture::SetSubImage3D(
    GLint level, GLint xoffset, GLint yoffset, GLuint zoffset, GLsizei width, GLsizei height, 
    GLsizei depth, GLenum format, GLenum type, const void *pPixels
) const
{
    glTextureSubImage3D(m_TextureID, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pPixels);
}

void Texture::BindUnit(GLuint unit) const
{
    glBindTextureUnit(unit, m_TextureID);
}