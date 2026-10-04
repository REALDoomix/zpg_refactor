#pragma once

#include <glad/gl.h>

class Shader {
public:
    Shader(GLenum type, const char* file);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint id() const { return shaderId; }

private:
    GLuint shaderId = 0;
};