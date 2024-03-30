#pragma once

#include <windows.h>
#include <gl/gl.h>

class FBO;

class FBOGuard {
public:
    ~FBOGuard();

private:
    FBOGuard(FBO &fbo, GLuint prevBuffer): m_fbo(fbo), m_prevBuffer(prevBuffer) {}
    FBO &m_fbo;
    GLuint m_prevBuffer;

    friend class FBO;
};

class FBO {
public:
    FBO(unsigned width, unsigned height);
    FBOGuard select();

    void bind();
    GLuint getTexture() const { return m_tex; }
    GLuint getDepthTexture() const { return m_depthTex; }

private:
    unsigned m_width;
    unsigned m_height;
    GLuint m_tex;
    GLuint m_fbo;
    GLuint m_depthTex;

    friend class FBOGuard;
};
