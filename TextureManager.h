#pragma once

#include "Texture.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TextureManager {
public:
    TextureManager() = default;
    ~TextureManager() = default;

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    TextureManager(TextureManager&&) = default;
    TextureManager& operator=(TextureManager&&) = default;

    std::shared_ptr<Texture> load(const std::string& name, const std::string& filePath);
    std::shared_ptr<Texture> loadCubeMap(const std::string& name, const std::vector<std::string>& faces);

    std::shared_ptr<Texture> get(const std::string& name) const;
    bool has(const std::string& name) const;
    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
