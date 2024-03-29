#include <windows.h>
#include <gl/gl.h>

#include "geometry.h"
#include "glext.h"
#include "glext-stubs.h"


Program compile_program(const char *shader_program, const char *fragment_program) {
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
    glCompileShader(fragment);
    // something with logging...

    // Attach shader objects to the program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Build program
    glLinkProgram(program);
    // something with logging...

    // Get rid of shaders (no more needed)
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);

    return Program{program}
}