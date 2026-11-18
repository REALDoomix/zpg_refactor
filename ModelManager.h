#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Model;

class ModelManager {
public:
    ModelManager() = default;
    ~ModelManager() = default;

    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    ModelManager(ModelManager&&) = default;
    ModelManager& operator=(ModelManager&&) = default;

    void add(const std::string& name, std::unique_ptr<Model> model);

    void load(const std::string& name, const float* vertices, size_t byteSize);
    void load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals);
    void load(const std::string& name, const float* vertices, size_t byteSize, bool hasNormals, bool normalBeforeColor);
    void loadWithUV(const std::string& name, const float* vertices, size_t byteSize);
    void loadOBJ(const std::string& name, const std::string& filePath);

    Model& get(const std::string& name);
    const Model& get(const std::string& name) const;

    bool has(const std::string& name) const;
    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Model>> models;
};
