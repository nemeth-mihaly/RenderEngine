#pragma once

#include <stddef.h>
#include <stdint.h>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// class Framebuffer
//-----------------------------------------------------------------------------

class Framebuffer
{
 public:
    Framebuffer();
    ~Framebuffer();

    bool Init();

    void Bind();
    void BindTexture();

    void Unbind();
    void UnbindTexture();

 private:
    GLuint      m_bufferID;
    GLuint      m_textureID;
    GLuint      m_renderBufferID;
};