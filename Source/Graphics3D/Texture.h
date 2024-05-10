#pragma once

#include "Common.h"

extern GLenum InternalFormat(int channelCount);
extern GLenum Format(int channelCount);

////////////////////////////////////////////////////
//  class Texture
////////////////////////////////////////////////////

class Texture
{
 public:
    Texture();
    ~Texture();

    void Create(GLenum target);
    void SetParamateri(GLenum pname, GLint param) const;
    void SetStorage2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) const;
    void SetSubImage2D(
        GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, 
        GLenum format, GLenum type, const void *pixels
    ) const;
    void BindUnit(GLuint unit) const;

 private:
    GLuint m_TextureID;
};

typedef std::shared_ptr<Texture> StrongTexturePtr;