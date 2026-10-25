#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Model; // Forward declaration

class ModelManager {
public:
    ModelManager() = default;
    ~ModelManager() = default;

    // Prevent copying (since models are managed via unique_ptr)
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    // Allow moving
    ModelManager(ModelManager&&) = default;
    ModelManager& operator=(ModelManager&&) = default;

    // 1. Direct addition (hand over an already constructed model)
    void add(const std::string& name, std::unique_ptr<Model> model);

    // 2. Overloaded load helpers (constructs the model internally)
    void load(const std::string& name, const float* vertices, size_t byteSize);
    void load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals);
    void load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals, bool normalBeforeColor);

    // Retrieval
    Model& get(const std::string& name);
    const Model& get(const std::string& name) const;

    // Utility
    bool has(const std::string& name) const;
    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Model>> models;
};