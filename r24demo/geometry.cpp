#include <windows.h>
#include <gl/gl.h>

#include "geometry.h"
#include "glext.h"
#include "glext-stubs.h"

#ifndef NDEBUG
#include <iostream>
#endif

inline void check_shader_log(GLuint shader)
{
#ifndef NDEBUG
    GLchar log[32768];
    GLsizei len;
    glGetShaderInfoLog(shader, sizeof(log), &len, log);
    if (log[0]) {
        MessageBoxA(0, log, "error", 0);
        exit(1);
    }
#endif
}

inline void check_program_log(GLuint program)
{
#ifndef NDEBUG
    GLchar log[32768];
    GLsizei len;
    glGetProgramInfoLog(program, sizeof(log), &len, log);
    if (log[0]) {
        MessageBoxA(0, log, "error", 0);
        exit(1);
    }
#endif
}

GLuint Geometry::compile_program(const char *shader_program, const char *fragment_program) {
    auto program  = glCreateProgram();
    auto vertex = glCreateShader(GL_VERTEX_SHADER);
    auto fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Set shaders source
    GLint shader_program_len = strlen(shader_program);
    GLint fragment_program_len = strlen(fragment_program);
    glShaderSource(vertex, 1, &shader_program, &shader_program_len);
    glShaderSource(fragment, 1, &fragment_program, &fragment_program_len);

    // Compile shaders
    glCompileShader(vertex);
    check_shader_log(vertex);
    
    glCompileShader(fragment);
    check_shader_log(fragment);

    // Attach shader objects to the program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Build program
    glLinkProgram(program);
    check_program_log(program);

    // Get rid of shaders (no more needed)
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);

    return program;
}

GLuint Geometry::create_vao() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void Geometry::use_program(GLuint program) {
    glUseProgram(program);
}

void Geometry::use_vao(GLuint vao) {
    glBindVertexArray(vao);
}

GLuint Geometry::create_vertex_buffer(void* p, size_t size) {
    GLuint buffer;
    GLuint err;
    glGenBuffers(1, &buffer);
    err = glGetError();
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    err = glGetError();
    glBufferData(GL_ARRAY_BUFFER, size, p, GL_STATIC_DRAW);
    err = glGetError();
    return buffer;
}

void Geometry::uniform(GLuint program, const char* name, const matrix4f& m) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, false, &m.m[0].x);
}

void Geometry::uniform(GLuint program, const char* name, const vec3& m) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform3fv(loc, 1, (GLfloat *) &m);
}

void Geometry::uniform(GLuint program, const char* name, const vec4& m) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform4fv(loc, 1, (GLfloat*)&m);
}

void Geometry::uniform(GLuint program, const char* name, const GLuint& m) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1i(loc, m);
}

void Geometry::texture(GLuint program, const char* name, GLuint texunit, const GLuint& tex) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1i(loc, texunit);
    glActiveTexture(GL_TEXTURE0 + texunit);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindSampler(texunit, 0);
}

void Geometry::uniform(GLuint program, const char* name, const float& m) {
    GLuint loc = glGetUniformLocation(program, name);
    glUniform1fv(loc, 1, &m);
}

int calc_stride(const Geometry::attrib_defs* defs) {
    int stride = 0;
    for (; defs->name; defs++) {
        stride += sizeof(float) * defs->dimensions;
    }

    return stride;
}

void Geometry::configure_attribs(const GLuint program, const Geometry::attrib_defs* defs, const GLuint buffer, int divisor) {
    if (!defs)
        return;

    auto stride = calc_stride(defs);
    auto offset = 0;
    GLuint err;

    for (; defs->name; defs++) {
        auto loc = glGetAttribLocation(program, defs->name);
        glEnableVertexAttribArray(loc);
        err = glGetError();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        err = glGetError();
        glVertexAttribPointer(loc, defs->dimensions, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        err = glGetError();
        glVertexAttribDivisor(loc, divisor);
        err = glGetError();

        offset += sizeof(float) * defs->dimensions;
    }
}

void Geometry::render(GLuint primitive, size_t vertices, size_t instances) {
    glDrawArraysInstanced(primitive, 0, vertices, instances);
    GLuint err = glGetError();

}