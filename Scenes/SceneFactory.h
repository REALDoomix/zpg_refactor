#pragma once
#include <memory>
#include "../Scene.h"
#include "../Model.h"
#include "../Camera.h"

class SceneFactory {
public:
    static std::unique_ptr<Scene> createTriangleScene(ShaderProgram& shader, Model& triangleModel);
    static std::unique_ptr<Scene> createSphereScene(ShaderProgram& shader, Model& sphereModel);
    static std::unique_ptr<Scene> createForestScene(ShaderProgram& basicShader, ShaderProgram& treeShader, ShaderProgram& clockShader, Model& treeModel, Model& bushModel, Model& sphereModel, Model& clockModel, Model& groundModel);
    static std::unique_ptr<Scene> createLoginScene(ShaderProgram& shader, Model& loginModel);
    static std::unique_ptr<Scene> createSolarSystemScene(ShaderProgram& shader, Model& sunModel, Model& earthModel, Model& moonModel);
};