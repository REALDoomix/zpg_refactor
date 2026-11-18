#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(size_t size, GLuint bindingPoint, GLenum usage)
    : size(size), bindingPoint(bindingPoint) {
    glGenBuffers(1, &uboId);
    glBindBuffer(GL_UNIFORM_BUFFER, uboId);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, usage);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboId);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() {
    if (uboId != 0) {
        glDeleteBuffers(1, &uboId);
        uboId = 0;
    }
}

void UniformBuffer::setData(const void* data, size_t dataSize, size_t offset) {
    glBindBuffer(GL_UNIFORM_BUFFER, uboId);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindBase() const {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboId);
}

void UniformBuffer::bind() const {
    glBindBuffer(GL_UNIFORM_BUFFER, uboId);
}

void UniformBuffer::unbind() const {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
