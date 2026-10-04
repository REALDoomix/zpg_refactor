#include "ShaderProgram.h"

#include <stdexcept>
#include <string>

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile)
    : vertexShader(GL_VERTEX_SHADER, vertexFile),
    fragmentShader(GL_FRAGMENT_SHADER, fragmentFile) {
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader.id());
    glAttachShader(programId, fragmentShader.id());
    glLinkProgram(programId);

    GLint success = GL_FALSE;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024] = {};
        glGetProgramInfoLog(programId, sizeof(infoLog), nullptr, infoLog);
        glDeleteProgram(programId);
        programId = 0;
        throw std::runtime_error(std::string("Shader program linking failed: ") + infoLog);
    }
}

ShaderProgram::~ShaderProgram() {
    if (programId) glDeleteProgram(programId);
}

void ShaderProgram::use() const {
    glUseProgram(programId);
}