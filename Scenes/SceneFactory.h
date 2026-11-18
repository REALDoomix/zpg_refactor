#pragma once
#include <memory>
#include "../Scene.h"
#include "../Model.h"
#include "../Light.h"
#include "../Camera.h"
#include "../ModelManager.h"
#include "../ShaderManager.h"
#include "../TextureManager.h"

class SceneFactory {
public:
    static std::unique_ptr<Scene> createTriangleScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createSphereScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createForestScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createLoginScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createSolarSystemScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createLightScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createNormalDemoScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createSpecularBleedDemoScene(ShaderManager& shaderManager, ModelManager& models);
    static std::unique_ptr<Scene> createNightForestScene(ShaderManager& shaderManager, ModelManager& models, std::shared_ptr<Light> flashlight = nullptr);

    // New texture scenes
    static std::unique_ptr<Scene> createTextureDemoScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures);
    static std::unique_ptr<Scene> createMultiTextureScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures);
    static std::unique_ptr<Scene> createShrekScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures);
    static std::unique_ptr<Scene> createSkyboxScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures);
    static std::unique_ptr<Scene> createEB408Scene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures);
};
