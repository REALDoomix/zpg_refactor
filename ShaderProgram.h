#pragma once

#include "Shader.h"

#include <glad/gl.h>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexFile, const char* fragmentFile);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;
    GLuint getProgram() const { return programId; }

private:
    Shader vertexShader;
    Shader fragmentShader;
    GLuint programId = 0;
};