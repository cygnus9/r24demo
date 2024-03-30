#pragma once

#include <windows.h>
#include <gl/gl.h>

#include "types.h"

class Geometry {
public:
    struct attrib_defs {
        const char* name;
        int dimensions;
    };

    static GLuint compile_program(const char* shader_program, const char* fragment_program);
    static GLuint create_vao();
    static void use_vao(GLuint vao);
    static void use_program(GLuint program);
    static GLuint create_vertex_buffer(void* data, size_t size);
    static void uniform_matrix(GLuint program, const char* name, const matrix4f& m);
    static void vertex_buffer(GLuint buffer);
    static void configure_attribs(const GLuint program, const Geometry::attrib_defs* defs, const GLuint buffer, int divisor);
    static void render(GLuint primitive, size_t vertices, size_t instances);
};
