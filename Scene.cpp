#include "Scene.h"
#include <iostream>


Scene::Scene()
    : shaderProgram("shaders/basic.vert", "shaders/basic.frag") {
    viewMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "viewMatrix");
    projectionMatrixLoc = glGetUniformLocation(shaderProgram.getProgram(), "projectionMatrix");

    if (viewMatrixLoc == -1) {
        std::cerr << "Warning: uniform 'viewMatrix' not found in shader program" << std::endl;
    }
    if (projectionMatrixLoc == -1) {
        std::cerr << "Warning: uniform 'projectionMatrix' not found in shader program" << std::endl;
    }
}

DrawableObject& Scene::addDrawable(Model& model) {
    drawables.emplace_back(model, shaderProgram);
    return drawables.back();
}

void Scene::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
    shaderProgram.use();
    shaderProgram.sendUniform(viewMatrixLoc, viewMatrix);
    shaderProgram.sendUniform(projectionMatrixLoc, projectionMatrix);

    for (const auto& drawable : drawables) {
        drawable.draw();
    }
}

void Scene::update(float deltaTime) {
    for (auto& drawable : drawables) {
        drawable.update(deltaTime);
    }
}