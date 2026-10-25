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