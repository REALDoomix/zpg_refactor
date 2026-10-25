#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class ShaderProgram;
class Camera;

class ShaderManager {
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    ShaderManager(ShaderManager&&) = default;
    ShaderManager& operator=(ShaderManager&&) = default;

    void add(const std::string& name, std::unique_ptr<ShaderProgram> shader);
    void load(const std::string& name, const char* vertexFile, const char* fragmentFile);

    ShaderProgram& get(const std::string& name);
    const ShaderProgram& get(const std::string& name) const;

    bool has(const std::string& name) const;
    void clear();

    // Attaches the camera to all loaded shaders in one call
    void setCamera(Camera* camera);

private:
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shaders;
    Camera* currentCamera = nullptr;
};