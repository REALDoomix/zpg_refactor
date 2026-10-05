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
// pro posilani napr angle do uniform promenne
void ShaderProgram::sendUniform(GLint location, float value) const {
    glUniform1f(location, value);
}
// pro poslani intu do promenne
void ShaderProgram::sendUniform(GLint location, int value) const {
    glUniform1i(location, value);
}
// posun na x a y
void ShaderProgram::sendUniform(GLint location, const glm::vec2& vec) const {
    glUniform2f(location, vec.x, vec.y);
}
// posun po vsech 3 osach
void ShaderProgram::sendUniform(GLint location, const glm::vec3& vec) const {
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::sendUniform(GLint location, const glm::vec4& vec) const {
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}