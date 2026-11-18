#pragma once

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Light.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Camera;

class Scene {
public:
    Scene();
    DrawableObject& addDrawable(Model& model, ShaderProgram& shaderProgram);

    std::shared_ptr<Light> addLight(const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& col = glm::vec3(1.0f));
    std::shared_ptr<Light> addLight(std::shared_ptr<ITransformation> transform, const glm::vec3& col = glm::vec3(1.0f));
    void addLight(std::shared_ptr<Light> light);

    const std::vector<std::shared_ptr<Light>>& getLights() const { return lights; }
    std::vector<DrawableObject>& getDrawables() { return drawables; }

    void update(float deltaTime);
    void draw() const;

private:
    std::vector<DrawableObject> drawables;
    std::vector<std::shared_ptr<Light>> lights;
};
