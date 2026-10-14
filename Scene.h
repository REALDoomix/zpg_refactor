#pragma once

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include <vector>
#include <glm/glm.hpp>

class Camera;

class Scene {
public:
    Scene();
    DrawableObject& addDrawable(Model& model, ShaderProgram& shaderProgram);
    void update(float deltaTime);
    void draw() const;
    std::vector<DrawableObject>& getDrawables() { return drawables; }


private:
    std::vector<DrawableObject> drawables;
};