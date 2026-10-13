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

private:
    Model& model;
    ShaderProgram& shaderProgram;
    std::unique_ptr<ITransformation> transformation;
    glm::vec3 color;

    GLint modelMatrixLoc;
    GLint colorLoc;
};