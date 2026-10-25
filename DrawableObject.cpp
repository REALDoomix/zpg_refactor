#include "DrawableObject.h"
#include <iostream>

#include <ctime>
#include <glm/gtc/constants.hpp>

#include "Model.h"
#include "ShaderProgram.h"

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(0.0f, 0.0f, 0.0f) {

    transformation = std::make_unique<TransformationComposite>(); // identity by default

    modelMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "modelMatrix");
    colorLoc = glGetUniformLocation(shaderProgram.getProgram(), "objectColor");
    normalMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "normalMatrix");

    if (modelMatrixLoc == -1) std::cerr << "Warning: uniform 'modelMatrix' not found" << std::endl;
    if (colorLoc == -1) std::cerr << "Warning: uniform 'objectColor' not found" << std::endl;
    if (normalMatrixLoc == -1) std::cerr << "Warning: uniform 'normalMatrix' not found" << std::endl;
}

DrawableObject& DrawableObject::setTransformation(std::unique_ptr<ITransformation> transform) {
    transformation = std::move(transform);
    return *this;
}

DrawableObject& DrawableObject::setColor(const glm::vec3& c) {
    color = c;
    return *this;
}

void DrawableObject::update(float deltaTime) {
    transformation->update(deltaTime);

    if (isClock) {
        std::time_t t = std::time(nullptr);
        std::tm localTime;
        localtime_s(&localTime, &t); // MSVC-safe; use localtime_r on non-Windows

        float hours = static_cast<float>(localTime.tm_hour % 12);
        float minutes = static_cast<float>(localTime.tm_min);
        float seconds = static_cast<float>(localTime.tm_sec);

        secondAngle = glm::radians(seconds * 6.0f);                          // 360/60
        minuteAngle = glm::radians(minutes * 6.0f + seconds * 0.1f);         // + creep from seconds
        hourAngle = glm::radians(hours * 30.0f + minutes * 0.5f);          // 360/12, + creep from minutes
    }
}

void DrawableObject::draw() const {
    shaderProgram.use();

    glm::mat4 modelMatrix = transformation->getMatrix();
    shaderProgram.sendUniform(modelMatrixLoc, modelMatrix);

    if (normalMatrixLoc != -1) {
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shaderProgram.sendUniform(normalMatrixLoc, normalMatrix);
    }


    shaderProgram.sendUniform(colorLoc, color);

    if (isClock) {
        shaderProgram.sendUniform(hourAngleLoc, hourAngle);
        shaderProgram.sendUniform(minuteAngleLoc, minuteAngle);
        shaderProgram.sendUniform(secondAngleLoc, secondAngle);
    }

    model.draw();
}


DrawableObject& DrawableObject::enableClockHands() {
    isClock = true;
    hourAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uHourAngle");
    minuteAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uMinuteAngle");
    secondAngleLoc = glGetUniformLocation(shaderProgram.getProgram(), "uSecondAngle");
    return *this;
}