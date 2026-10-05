#pragma once

#include "Shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glad/gl.h>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexFile, const char* fragmentFile);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;
    GLuint getProgram() const { return programId; }

    // Přetížené sendUniform funkce
    void sendUniform(GLint location, float value) const;
    void sendUniform(GLint location, int value) const;
    void sendUniform(GLint location, const glm::vec2& vec) const;
    void sendUniform(GLint location, const glm::vec3& vec) const;
    void sendUniform(GLint location, const glm::vec4& vec) const;

private:
    Shader vertexShader;
    Shader fragmentShader;
    GLuint programId = 0;
};