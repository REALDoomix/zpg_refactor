// DrawableObject.h
#pragma once
#include "Transformations.h"

#include "Model.h"
#include "ShaderProgram.h"

class DrawableObject {
public:
    DrawableObject(Model& model, ShaderProgram& shaderProgram);

    void update(float deltaTime);
    void draw() const;

    DrawableObject& setTransformation(std::unique_ptr<ITransformation> transform);
    DrawableObject& setColor(const glm::vec3& c);
    DrawableObject& enableClockHands();

private:
    Model& model;
    ShaderProgram& shaderProgram;
    std::unique_ptr<ITransformation> transformation;
    glm::vec3 color;

    bool isClock = false;
    GLint hourAngleLoc = -1, minuteAngleLoc = -1, secondAngleLoc = -1;
    float hourAngle = 0.0f, minuteAngle = 0.0f, secondAngle = 0.0f;

    GLint modelMatrixLoc;
    GLint colorLoc;
};