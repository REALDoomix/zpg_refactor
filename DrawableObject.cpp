#include "DrawableObject.h"
#include <iostream>

#include "Model.h"
#include "ShaderProgram.h"

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram)
    : model(model), shaderProgram(shaderProgram), color(0.0f, 0.0f, 0.0f) {

    transformation = std::make_unique<TransformationComposite>(); // identity by default

    modelMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "modelMatrix");
    colorLoc = glGetUniformLocation(shaderProgram.getProgram(), "objectColor");

    if (modelMatrixLoc == -1) std::cerr << "Warning: uniform 'modelMatrix' not found" << std::endl;
    if (colorLoc == -1) std::cerr << "Warning: uniform 'objectColor' not found" << std::endl;
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
}

void DrawableObject::draw() const {
    shaderProgram.use();
    shaderProgram.sendUniform(modelMatrixLoc, transformation->getMatrix());
    shaderProgram.sendUniform(colorLoc, color);
    model.draw();
}