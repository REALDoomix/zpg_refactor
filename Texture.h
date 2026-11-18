#pragma once

#include <glad/gl.h>
#include <string>
#include <vector>

class Texture {
public:
    // Load 2D texture from an image file
    explicit Texture(const std::string& filePath);

    // Load CubeMap from 6 face image files (+X, -X, +Y, -Y, +Z, -Z)
    explicit Texture(const std::vector<std::string>& faces);

    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void bind(GLuint unit = 0) const;
    void unbind() const;

    GLuint getId() const { return textureId; }
    GLenum getTarget() const { return target; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    GLuint textureId = 0;
    GLenum target = GL_TEXTURE_2D;
    int width = 0;
    int height = 0;
    int channels = 0;
};
