#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "glext.h"

extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers;

void init_glext_stubs();
