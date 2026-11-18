#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"

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

void ShaderProgram::setCamera(Camera* newCamera) {
    camera = newCamera;
    viewMatrixLoc = glGetUniformLocation(programId, "viewMatrix");
    projectionMatrixLoc = glGetUniformLocation(programId, "projectionMatrix");
    camera->attach(this);
    onCameraChanged(); // push current matrices immediately, don't wait for the first camera movement
}

void ShaderProgram::onCameraChanged() {
    if (!camera) return;
    use();
    sendUniform(viewMatrixLoc, camera->getViewMatrix());
    sendUniform(projectionMatrixLoc, camera->getProjectionMatrix());

    if (viewPositionLoc == -1) {
        viewPositionLoc = glGetUniformLocation(programId, "viewPosition");
    }
    if (viewPositionLoc != -1) {
        sendUniform(viewPositionLoc, camera->getPos());
    }
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

void ShaderProgram::sendUniform(GLint location, const glm::mat3& mat) const {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::sendUniform(GLint location, const glm::vec4& vec) const {
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::sendUniform(GLint location, const glm::mat4& mat) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setLightPosition(const glm::vec3& position) {
    if (lightPositionLoc == -1) {
        lightPositionLoc = glGetUniformLocation(programId, "lightPosition");
    }
    use();
    sendUniform(lightPositionLoc, position);
}

void ShaderProgram::sendLights(const std::vector<std::shared_ptr<Light>>& lights) const {
    if (lights.empty()) return;

    // Fallback pro shadery, které mají pouze 'lightPosition'
    GLint singleLightLoc = glGetUniformLocation(programId, "lightPosition");
    if (singleLightLoc != -1) {
        use();
        sendUniform(singleLightLoc, lights[0]->getPosition());
    }

    // Podpora pro pole světel (např. shaders/newPhong.frag)
    GLint numLightsLoc = glGetUniformLocation(programId, "numLights");
    if (numLightsLoc == -1) {
        return;
    }

    use();
    int count = static_cast<int>(lights.size());
    if (count > 8) count = 8;
    sendUniform(numLightsLoc, count);

    for (int i = 0; i < count; ++i) {
        std::string base = "lights[" + std::to_string(i) + "].";

        GLint posLoc = glGetUniformLocation(programId, (base + "position").c_str());
        GLint colLoc = glGetUniformLocation(programId, (base + "color").c_str());
        GLint typeLoc = glGetUniformLocation(programId, (base + "type").c_str());
        GLint dirLoc = glGetUniformLocation(programId, (base + "direction").c_str());
        GLint constLoc = glGetUniformLocation(programId, (base + "constant").c_str());
        GLint linLoc = glGetUniformLocation(programId, (base + "linear").c_str());
        GLint quadLoc = glGetUniformLocation(programId, (base + "quadratic").c_str());

        if (posLoc != -1) sendUniform(posLoc, lights[i]->getPosition());
        if (colLoc != -1) sendUniform(colLoc, lights[i]->color);
        if (typeLoc != -1) sendUniform(typeLoc, static_cast<int>(lights[i]->type));
        if (dirLoc != -1) sendUniform(dirLoc, lights[i]->direction);
        if (constLoc != -1) sendUniform(constLoc, lights[i]->constant);
        if (linLoc != -1) sendUniform(linLoc, lights[i]->linear);
        if (quadLoc != -1) sendUniform(quadLoc, lights[i]->quadratic);
    }
}
