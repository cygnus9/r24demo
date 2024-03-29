#include "fbo.h"
#include "glext-stubs.h"

FBO::FBO(unsigned width, unsigned height):
    m_width(width),
    m_height(height)
{
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);

    glGenTextures(1, &m_depthTex);
    glBindTexture(GL_TEXTURE_2D, m_depthTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthTex, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

FBOGuard FBO::select() {
    glPushAttrib(GL_VIEWPORT_BIT);
    GLint prevBuffer;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prevBuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
    glViewport(0, 0, m_width, m_height);
    return FBOGuard(*this, prevBuffer);
}

FBOGuard::~FBOGuard() {
    glBindTexture(GL_TEXTURE_2D, m_fbo.m_tex);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_prevBuffer);
    glPopAttrib();
}

void FBO::bind() {
    glBindTexture(GL_TEXTURE_2D, m_tex);
}
