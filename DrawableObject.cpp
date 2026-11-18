#include "DrawableObject.h"
#include <iostream>

#include <ctime>
#include <glm/gtc/constants.hpp>

#include "Model.h"
#include "ShaderProgram.h"

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(0.0f, 0.0f, 0.0f) {

    transformation = std::make_shared<TransformationComposite>(); // identity by default

    modelMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "modelMatrix");
    colorLoc = glGetUniformLocation(shaderProgram.getProgram(), "objectColor");
    normalMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "normalMatrix");
}

DrawableObject& DrawableObject::setTransformation(std::shared_ptr<ITransformation> transform) {
    transformation = std::move(transform);
    return *this;
}

DrawableObject& DrawableObject::setColor(const glm::vec3& c) {
    color = c;
    return *this;
}

DrawableObject& DrawableObject::setTexture(std::shared_ptr<Texture> tex, GLuint unit, const std::string& uniformName) {
    textureBindings.clear();
    textureBindings.push_back({ tex, unit, uniformName });
    return *this;
}

DrawableObject& DrawableObject::addTexture(std::shared_ptr<Texture> tex, GLuint unit, const std::string& uniformName) {
    textureBindings.push_back({ tex, unit, uniformName });
    return *this;
}

DrawableObject& DrawableObject::clearTextures() {
    textureBindings.clear();
    return *this;
}

DrawableObject& DrawableObject::setSkyboxMode(bool enabled) {
    isSkyboxMode = enabled;
    return *this;
}

void DrawableObject::update(float deltaTime) {
    transformation->update(deltaTime);

    if (isClock) {
        std::time_t t = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &t);

        float hours = static_cast<float>(localTime.tm_hour % 12);
        float minutes = static_cast<float>(localTime.tm_min);
        float seconds = static_cast<float>(localTime.tm_sec);

        secondAngle = glm::radians(seconds * 6.0f);
        minuteAngle = glm::radians(minutes * 6.0f + seconds * 0.1f);
        hourAngle = glm::radians(hours * 30.0f + minutes * 0.5f);
    }
}

void DrawableObject::draw() const {
    shaderProgram.use();

    glm::mat4 modelMatrix = transformation->getMatrix();
    if (modelMatrixLoc != -1) {
        shaderProgram.sendUniform(modelMatrixLoc, modelMatrix);
    }

    if (normalMatrixLoc != -1) {
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shaderProgram.sendUniform(normalMatrixLoc, normalMatrix);
    }

    if (colorLoc != -1) {
        shaderProgram.sendUniform(colorLoc, color);
    }

    // Bind and configure all attached textures
    for (const auto& binding : textureBindings) {
        if (binding.texture) {
            binding.texture->bind(binding.unit);
            GLint loc = glGetUniformLocation(shaderProgram.getProgram(), binding.uniformName.c_str());
            if (loc != -1) {
                shaderProgram.sendUniform(loc, static_cast<int>(binding.unit));
            }
        }
    }

    if (isClock) {
        shaderProgram.sendUniform(hourAngleLoc, hourAngle);
        shaderProgram.sendUniform(minuteAngleLoc, minuteAngle);
        shaderProgram.sendUniform(secondAngleLoc, secondAngle);
    }

    if (isSkyboxMode) {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
    }

    model.draw();

    if (isSkyboxMode) {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
}

DrawableObject& DrawableObject::enableClockHands() {
    isClock = true;
    hourAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uHourAngle");
    minuteAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uMinuteAngle");
    secondAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uSecondAngle");
    return *this;
}
