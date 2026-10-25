#pragma once
#include <memory>
#include "../Scene.h"
#include "../Model.h"
#include "../Camera.h"
#include "../ModelManager.h"
#include "../ShaderManager.h"
class SceneFactory {
public:
    static std::unique_ptr<Scene> createTriangleScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createSphereScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createForestScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createLoginScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createSolarSystemScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createLightScene(ShaderManager& shaderManager, ModelManager& models);
};