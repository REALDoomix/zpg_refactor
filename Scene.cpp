#include "Scene.h"


Scene::Scene()
    : shaderProgram("shaders/basic.vert", "shaders/basic.frag") {
}

DrawableObject& Scene::addDrawable(Model& model) {
    drawables.emplace_back(model, shaderProgram);
    return drawables.back();
}

void Scene::draw() const {
    for (const auto& drawable : drawables) {
        drawable.draw();
    }
}