#pragma once

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include <vector>
#include <glm/glm.hpp>

class Scene {
public:
    Scene();
    DrawableObject& addDrawable(Model& model);
    void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;
    std::vector<DrawableObject>& getDrawables() { return drawables; }

    void update(float deltaTime);

private:
    ShaderProgram shaderProgram;
    std::vector<DrawableObject> drawables;
    GLint viewMatrixLoc;
    GLint projectionMatrixLoc;
};