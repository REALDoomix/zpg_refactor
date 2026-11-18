#include "Scene.h"
#include <iostream>

Scene::Scene() {
}

DrawableObject& Scene::addDrawable(Model& model, ShaderProgram& shaderProgram) {
    drawables.emplace_back(model, shaderProgram);
    return drawables.back();
}

std::shared_ptr<Light> Scene::addLight(const glm::vec3& pos, const glm::vec3& col) {
    auto light = std::make_shared<Light>(pos, col);
    lights.push_back(light);
    return light;
}

std::shared_ptr<Light> Scene::addLight(std::shared_ptr<ITransformation> transform, const glm::vec3& col) {
    auto light = std::make_shared<Light>(transform, col);
    lights.push_back(light);
    return light;
}

void Scene::addLight(std::shared_ptr<Light> light) {
    if (light) {
        lights.push_back(light);
    }
}

void Scene::draw() const {
    for (const auto& drawable : drawables) {
        if (!lights.empty()) {
            drawable.getShaderProgram().sendLights(lights);
        }
        drawable.draw();
    }
}

void Scene::update(float deltaTime) {
    for (auto& drawable : drawables) {
        drawable.update(deltaTime);
    }
}
