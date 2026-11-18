#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <cstddef>

class UniformBuffer {
public:
    UniformBuffer(size_t size, GLuint bindingPoint, GLenum usage = GL_DYNAMIC_DRAW);
    ~UniformBuffer();

    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;

    void setData(const void* data, size_t size, size_t offset = 0);
    void bindBase() const;
    void bind() const;
    void unbind() const;

    GLuint getId() const { return uboId; }
    GLuint getBindingPoint() const { return bindingPoint; }

private:
    GLuint uboId = 0;
    GLuint bindingPoint = 0;
    size_t size = 0;
};
