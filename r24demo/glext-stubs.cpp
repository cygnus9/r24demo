#include "glext-stubs.h"

PROC _gl_procs[33];

const char *procnames =
    "glActiveTexture\0"
    "glAttachShader\0"
    "glBindBuffer\0"
    "glBindFramebufferEXT\0"
    "glBindSampler\0"
    "glBindVertexArray\0"
    "glBufferData\0"
    "glCompileShader\0"
    "glCreateProgram\0"
    "glCreateShader\0"
    "glDetachShader\0"
    "glDrawArraysInstanced\0"
    "glEnableVertexAttribArray\0"
    "glFramebufferTexture2DEXT\0"
    "glGenBuffers\0"
    "glGenerateMipmap\0"
    "glGenFramebuffers\0"
    "glGenVertexArrays\0"
    "glGetAttribLocation\0"
    "glGetProgramInfoLog\0"
    "glGetShaderInfoLog\0"
    "glGetUniformLocation\0"
    "glLinkProgram\0"
    "glShaderSource\0"
    "glUniform1fv\0"
    "glUniform1i\0"
    "glUniform3fv\0"
    "glUniform4fv\0"
    "glUniformMatrix4fv\0"
    "glUseProgram\0"
    "glVertexAttribDivisor\0"
    "glVertexAttribPointer\0"
    "glWindowPos2i\0"
    ;

void init_glext_stubs() {
    int n = 0;
    const char *p = procnames;
    while (*p) {
        _gl_procs[n++] = wglGetProcAddress(p);
        p = strchr(p, '\0') + 1;
    }
}
