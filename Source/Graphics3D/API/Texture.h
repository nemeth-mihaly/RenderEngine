#pragma once

#include "Common.h"

extern GLenum TextureInternalFormat(int channelCount);
extern GLenum TextureFormat(int channelCount);

////////////////////////////////////////////////////
//  class Texture
////////////////////////////////////////////////////

class Texture
{
 public:
    Texture();
    ~Texture();

    void Create(GLenum target);

    void SetActive(GLboolean bActive = GL_TRUE) const;
    
    void SetParamateri(GLenum pname, GLint param) const;

    void SetImage2DForSpecialTarget(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, const void* pPixels) const;
    void SetImage2D(GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, const void* pPixels) const;
    
    void GenerateMipmap() const;

    void SetActiveUnit(GLuint unit = 0) const;

 private:
    GLuint      m_TextureID;
    GLenum      m_TextureTarget;
};

typedef     std::shared_ptr<Texture>        StrongTexturePtr;