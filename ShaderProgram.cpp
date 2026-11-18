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

    // Podpora pro pole světel (numLights nebo numberOfLights dle zadání)
    GLint numLightsLoc = glGetUniformLocation(programId, "numLights");
    if (numLightsLoc == -1) {
        numLightsLoc = glGetUniformLocation(programId, "numberOfLights");
    }
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
        GLint diffLoc = glGetUniformLocation(programId, (base + "diffuse").c_str());
        GLint specLoc = glGetUniformLocation(programId, (base + "specular").c_str());
        GLint typeLoc = glGetUniformLocation(programId, (base + "type").c_str());
        GLint dirLoc = glGetUniformLocation(programId, (base + "direction").c_str());
        GLint constLoc = glGetUniformLocation(programId, (base + "constant").c_str());
        GLint linLoc = glGetUniformLocation(programId, (base + "linear").c_str());
        GLint quadLoc = glGetUniformLocation(programId, (base + "quadratic").c_str());
        GLint cutoffLoc = glGetUniformLocation(programId, (base + "spotCutoff").c_str());

        // Pokud je světlo vypnuté (např. zhasnutá baterka), předáme barvu 0
        glm::vec3 effectiveColor = lights[i]->enabled ? lights[i]->color : glm::vec3(0.0f);

        if (posLoc != -1) sendUniform(posLoc, lights[i]->getPosition());
        if (colLoc != -1) sendUniform(colLoc, effectiveColor);
        if (diffLoc != -1) sendUniform(diffLoc, glm::vec4(effectiveColor, 1.0f));
        if (specLoc != -1) sendUniform(specLoc, glm::vec4(effectiveColor, 1.0f));
        if (typeLoc != -1) sendUniform(typeLoc, static_cast<int>(lights[i]->type));
        if (dirLoc != -1) sendUniform(dirLoc, lights[i]->direction);
        if (constLoc != -1) sendUniform(constLoc, lights[i]->constant);
        if (linLoc != -1) sendUniform(linLoc, lights[i]->linear);
        if (quadLoc != -1) sendUniform(quadLoc, lights[i]->quadratic);
        if (cutoffLoc != -1) sendUniform(cutoffLoc, lights[i]->spotCutoff);
    }
}

void ShaderProgram::bindUniformBlock(const std::string& blockName, GLuint bindingPoint) const {
    GLuint blockIndex = glGetUniformBlockIndex(programId, blockName.c_str());
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(programId, blockIndex, bindingPoint);
    }
}
