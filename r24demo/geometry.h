#pragma once

#include <windows.h>
#include <gl/gl.h>

struct Program {
    GLuint handle;
};

Program compile_program(const char *shader_program, , const char *fragment_program);

struct RenderInfo {
    Program program;

}
