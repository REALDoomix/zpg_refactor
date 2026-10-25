#include "ShaderManager.h"
#include "ShaderProgram.h"
#include <stdexcept>

void ShaderManager::add(const std::string& name, std::unique_ptr<ShaderProgram> shader) {
    if (currentCamera) {
        shader->setCamera(currentCamera);
    }
    shaders[name] = std::move(shader);
}

void ShaderManager::load(const std::string& name, const char* vertexFile, const char* fragmentFile) {
    auto shader = std::make_unique<ShaderProgram>(vertexFile, fragmentFile);
    if (currentCamera) {
        shader->setCamera(currentCamera);
    }
    shaders[name] = std::move(shader);
}

ShaderProgram& ShaderManager::get(const std::string& name) {
    auto it = shaders.find(name);
    if (it == shaders.end()) {
        throw std::runtime_error("ShaderManager: Shader '" + name + "' not found!");
    }
    return *(it->second);
}

const ShaderProgram& ShaderManager::get(const std::string& name) const {
    auto it = shaders.find(name);
    if (it == shaders.end()) {
        throw std::runtime_error("ShaderManager: Shader '" + name + "' not found!");
    }
    return *(it->second);
}

bool ShaderManager::has(const std::string& name) const {
    return shaders.find(name) != shaders.end();
}

void ShaderManager::clear() {
    shaders.clear();
}

void ShaderManager::setCamera(Camera* camera) {
    currentCamera = camera;
    if (!currentCamera) return;

    for (auto& [name, shader] : shaders) {
        shader->setCamera(currentCamera);
    }
}