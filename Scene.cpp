#include "Scene.h"
#include <iostream>


Scene::Scene(){
    
}

DrawableObject& Scene::addDrawable(Model& model, ShaderProgram& shaderProgram) {
    drawables.emplace_back(model, shaderProgram);
    return drawables.back();
}

void Scene::draw() const {
    for (const auto& drawable : drawables) {
        drawable.draw();
    }
}

void Scene::update(float deltaTime) {
    for (auto& drawable : drawables) {
        drawable.update(deltaTime);
    }
}