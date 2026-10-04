#include "Shader.h"

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

Shader::Shader(GLenum type, const char* file) {
    std::ifstream shaderFile(file);
    if (!shaderFile) {
        throw std::runtime_error(std::string("Unable to open shader file: ") + file);
    }

    const std::string source(std::istreambuf_iterator<char>(shaderFile), {});
    const char* sourceData = source.c_str();

    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &sourceData, nullptr);
    glCompileShader(shaderId);

    GLint success = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024] = {};
        glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog);
        glDeleteShader(shaderId);
        shaderId = 0;
        throw std::runtime_error(std::string("Shader compilation failed: ") + infoLog);
    }
}

Shader::~Shader() {
    if (shaderId) glDeleteShader(shaderId);
}