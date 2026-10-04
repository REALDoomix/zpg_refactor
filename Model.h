#pragma once

#include <glad/gl.h>

#include <cstddef>

class Model {
public:
    Model(const float* vertices, std::size_t byteSize);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void draw() const;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
};