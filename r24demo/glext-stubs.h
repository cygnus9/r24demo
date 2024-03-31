#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

typedef void (APIENTRYP PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);

extern PROC _gl_procs[33];

void init_glext_stubs();


#define _glprocoffs_glActiveTexture  0
#define _glprocoffs_glAttachShader  1
#define _glprocoffs_glBindBuffer  2
#define _glprocoffs_glBindFramebufferEXT  3
#define _glprocoffs_glBindSampler  4
#define _glprocoffs_glBindVertexArray  5
#define _glprocoffs_glBufferData  6
#define _glprocoffs_glCompileShader  7
#define _glprocoffs_glCreateProgram  8
#define _glprocoffs_glCreateShader  9
#define _glprocoffs_glDetachShader  10
#define _glprocoffs_glDrawArraysInstanced    11
#define _glprocoffs_glEnableVertexAttribArray  12
#define _glprocoffs_glFramebufferTexture2DEXT  13
#define _glprocoffs_glGenBuffers  14
#define _glprocoffs_glGenerateMipmap  15
#define _glprocoffs_glGenFramebuffers  16
#define _glprocoffs_glGenVertexArrays  17
#define _glprocoffs_glGetAttribLocation  18
#define _glprocoffs_glGetProgramInfoLog  19
#define _glprocoffs_glGetShaderInfoLog  20
#define _glprocoffs_glGetUniformLocation  21
#define _glprocoffs_glLinkProgram  22
#define _glprocoffs_glShaderSource  23
#define _glprocoffs_glUniform1fv  24
#define _glprocoffs_glUniform1i  25
#define _glprocoffs_glUniform3fv  26
#define _glprocoffs_glUniform4fv  27
#define _glprocoffs_glUniformMatrix4fv    28
#define _glprocoffs_glUseProgram  29
#define _glprocoffs_glVertexAttribDivisor  30
#define _glprocoffs_glVertexAttribPointer  31
#define _glprocoffs_glWindowPos2i  32


#define glActiveTexture ((PFNGLACTIVETEXTUREPROC)_gl_procs[_glprocoffs_glActiveTexture])
#define glAttachShader ((PFNGLATTACHSHADERPROC)_gl_procs[_glprocoffs_glAttachShader])
#define glBindBuffer ((PFNGLBINDBUFFERPROC)_gl_procs[_glprocoffs_glBindBuffer])
#define glBindFramebufferEXT ((PFNGLBINDFRAMEBUFFEREXTPROC)_gl_procs[_glprocoffs_glBindFramebufferEXT])
#define glBindSampler ((PFNGLBINDSAMPLERPROC)_gl_procs[_glprocoffs_glBindSampler])
#define glBindVertexArray ((PFNGLBINDVERTEXARRAYPROC)_gl_procs[_glprocoffs_glBindVertexArray])
#define glBufferData ((PFNGLBUFFERDATAPROC)_gl_procs[_glprocoffs_glBufferData])
#define glCompileShader ((PFNGLCOMPILESHADERPROC)_gl_procs[_glprocoffs_glCompileShader])
#define glCreateProgram ((PFNGLCREATEPROGRAMPROC)_gl_procs[_glprocoffs_glCreateProgram])
#define glCreateShader ((PFNGLCREATESHADERPROC)_gl_procs[_glprocoffs_glCreateShader])
#define glDetachShader ((PFNGLDETACHSHADERPROC)_gl_procs[_glprocoffs_glDetachShader])
#define glDrawArraysInstanced ((PFNGLDRAWARRAYSINSTANCEDEXTPROC)_gl_procs[_glprocoffs_glDrawArraysInstanced])
#define glEnableVertexAttribArray ((PFNGLENABLEVERTEXATTRIBARRAYPROC)_gl_procs[_glprocoffs_glEnableVertexAttribArray])
#define glFramebufferTexture2DEXT ((PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)_gl_procs[_glprocoffs_glFramebufferTexture2DEXT])
#define glGenBuffers ((PFNGLGENBUFFERSPROC)_gl_procs[_glprocoffs_glGenBuffers])
#define glGenerateMipmap ((PFNGLGENERATEMIPMAPPROC)_gl_procs[_glprocoffs_glGenerateMipmap])
#define glGenFramebuffers ((PFNGLGENFRAMEBUFFERSEXTPROC)_gl_procs[_glprocoffs_glGenFramebuffers])
#define glGenVertexArrays ((PFNGLGENVERTEXARRAYSPROC)_gl_procs[_glprocoffs_glGenVertexArrays])
#define glGetAttribLocation ((PFNGLGETATTRIBLOCATIONPROC)_gl_procs[_glprocoffs_glGetAttribLocation])
#define glGetProgramInfoLog ((PFNGLGETPROGRAMINFOLOGPROC)_gl_procs[_glprocoffs_glGetProgramInfoLog])
#define glGetShaderInfoLog ((PFNGLGETSHADERINFOLOGPROC)_gl_procs[_glprocoffs_glGetShaderInfoLog])
#define glGetUniformLocation ((PFNGLGETUNIFORMLOCATIONPROC)_gl_procs[_glprocoffs_glGetUniformLocation])
#define glLinkProgram ((PFNGLLINKPROGRAMPROC)_gl_procs[_glprocoffs_glLinkProgram])
#define glShaderSource ((PFNGLSHADERSOURCEPROC)_gl_procs[_glprocoffs_glShaderSource])
#define glUniform1fv ((PFNGLUNIFORM1FVPROC)_gl_procs[_glprocoffs_glUniform1fv])
#define glUniform1i ((PFNGLUNIFORM1IPROC)_gl_procs[_glprocoffs_glUniform1i])
#define glUniform3fv ((PFNGLUNIFORM3FVPROC)_gl_procs[_glprocoffs_glUniform3fv])
#define glUniform4fv ((PFNGLUNIFORM4FVPROC)_gl_procs[_glprocoffs_glUniform4fv])
#define glUniformMatrix4fv ((PFNGLUNIFORMMATRIX4FVPROC)_gl_procs[_glprocoffs_glUniformMatrix4fv])
#define glUseProgram ((PFNGLUSEPROGRAMPROC)_gl_procs[_glprocoffs_glUseProgram])
#define glVertexAttribDivisor ((PFNGLVERTEXATTRIBDIVISORARBPROC)_gl_procs[_glprocoffs_glVertexAttribDivisor])
#define glVertexAttribPointer ((PFNGLVERTEXATTRIBPOINTERPROC)_gl_procs[_glprocoffs_glVertexAttribPointer])
#define glWindowPos2i ((PFNGLWINDOWPOS2IPROC)_gl_procs[_glprocoffs_glWindowPos2i])
