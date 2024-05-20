#include "Graphics3D/OpenGL/Texture.h"

GLenum TextureInternalFormat(int channelCount)
{
    int internalformat;

    switch (channelCount)
    {
        case 3: internalformat = GL_RGB8;       break;
        case 4: internalformat = GL_RGBA8;      break;

        default:
            assert(true)
        ;
    }

    return internalformat;
}

GLenum TextureFormat(int channelCount)
{
    int format;

    switch (channelCount)
    {
        case 3: format = GL_RGB;       break;
        case 4: format = GL_RGBA;      break;

        default:
            assert(true)
        ;
    }

    return format;
}

////////////////////////////////////////////////////
//  Texture Implementation
////////////////////////////////////////////////////

Texture::Texture()
{
    m_TextureID = 0xffffffff;
}

Texture::~Texture()
{
    if (m_TextureID != 0xffffffff) 
        glDeleteTextures(1, &m_TextureID)
    ;
}

void Texture::Create(GLenum target)
{
    m_TextureTarget = target;
    glGenTextures(1, &m_TextureID);
    glBindTexture(m_TextureTarget, m_TextureID);
}

void Texture::SetActive(GLboolean bActive) const
{
    glBindTexture(m_TextureTarget, (bActive ? m_TextureID : 0));
}

void Texture::SetParamateri(GLenum pname, GLint param) const
{
    glTexParameteri(m_TextureTarget, pname, param);
}

void Texture::SetImage2DForSpecialTarget(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, const void* pPixels) const
{
    glTexImage2D(target, level, internalformat, width, height, border, format, GL_UNSIGNED_BYTE, pPixels);
}

void Texture::SetImage2D(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, const void* pPixels) const
{
    glTexImage2D(m_TextureTarget, level, internalformat, width, height, border, format, GL_UNSIGNED_BYTE, pPixels);
}

void Texture::GenerateMipmap() const
{
    glGenerateMipmap(m_TextureTarget);
}

void Texture::SetActiveUnit(GLuint unit) const
{
    glBindTexture(m_TextureTarget, m_TextureID);
    glActiveTexture(GL_TEXTURE0 + unit);
}