#include "TextureManager.h"
#include <stdexcept>

std::shared_ptr<Texture> TextureManager::load(const std::string& name, const std::string& filePath) {
    auto tex = std::make_shared<Texture>(filePath);
    textures[name] = tex;
    return tex;
}

std::shared_ptr<Texture> TextureManager::loadCubeMap(const std::string& name, const std::vector<std::string>& faces) {
    auto tex = std::make_shared<Texture>(faces);
    textures[name] = tex;
    return tex;
}

std::shared_ptr<Texture> TextureManager::get(const std::string& name) const {
    auto it = textures.find(name);
    if (it == textures.end()) {
        throw std::runtime_error("TextureManager: Texture '" + name + "' not found!");
    }
    return it->second;
}

bool TextureManager::has(const std::string& name) const {
    return textures.find(name) != textures.end();
}

void TextureManager::clear() {
    textures.clear();
}
