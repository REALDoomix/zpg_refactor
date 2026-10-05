#pragma once

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include <vector>

class Scene {
public:
    Scene();

    void draw() const;
    DrawableObject& addDrawable(Model& model);
    std::vector<DrawableObject>& getDrawables() { return drawables; }

private:
    ShaderProgram shaderProgram;
    std::vector<DrawableObject> drawables;
};