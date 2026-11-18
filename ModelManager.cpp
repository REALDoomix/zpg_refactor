#include "ModelManager.h"
#include "Model.h"
#include <stdexcept>

void ModelManager::add(const std::string& name, std::unique_ptr<Model> model) {
    models[name] = std::move(model);
}

void ModelManager::load(const std::string& name, const float* vertices, size_t byteSize) {
    models[name] = std::make_unique<Model>(vertices, byteSize);
}

void ModelManager::load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals) {
    models[name] = std::make_unique<Model>(vertices, byteSize, hasNormals);
}

void ModelManager::load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals, bool normalBeforeColor) {
    models[name] = std::make_unique<Model>(vertices, byteSize, hasNormals, normalBeforeColor);
}

void ModelManager::loadWithUV(const std::string& name, const float* vertices, size_t byteSize) {
    models[name] = std::make_unique<Model>(vertices, byteSize, Model::VertexLayout::Pos3_Norm3_UV2);
}

void ModelManager::loadOBJ(const std::string& name, const std::string& filePath) {
    auto model = Model::fromOBJ(filePath);
    if (!model) {
        throw std::runtime_error("ModelManager: Failed to load OBJ model from '" + filePath + "'");
    }
    models[name] = std::move(model);
}

Model& ModelManager::get(const std::string& name) {
    auto it = models.find(name);
    if (it == models.end()) {
        throw std::runtime_error("ModelManager: Model '" + name + "' not found!");
    }
    return *(it->second);
}

const Model& ModelManager::get(const std::string& name) const {
    auto it = models.find(name);
    if (it == models.end()) {
        throw std::runtime_error("ModelManager: Model '" + name + "' not found!");
    }
    return *(it->second);
}

bool ModelManager::has(const std::string& name) const {
    return models.find(name) != models.end();
}

void ModelManager::clear() {
    models.clear();
}
