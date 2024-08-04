#include "Framebuffer.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

Framebuffer::Framebuffer()
{
    m_bufferID = 0xFFFFFFFF;
    m_textureID = 0xFFFFFFFF;
    m_renderBufferID = 0xFFFFFFFF;
}

Framebuffer::~Framebuffer()
{
    if (m_renderBufferID != 0xFFFFFFFF)
    {
        glDeleteRenderbuffers(1, &m_renderBufferID);
    }

    if (m_textureID != 0xFFFFFFFF)
    {
        glDeleteTextures(1, &m_textureID);
    }

    if (m_bufferID != 0xFFFFFFFF)
    {
        glDeleteFramebuffers(1, &m_bufferID);
    }
}

bool Framebuffer::Init()
{
    glGenFramebuffers(1, &m_bufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_pApp->GetWindowSize().x, g_pApp->GetWindowSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

    glGenRenderbuffers(1, &m_renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_pApp->GetWindowSize().x, g_pApp->GetWindowSize().y);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferID);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Framebuffer::UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}