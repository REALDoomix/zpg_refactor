#pragma once

#include <glad/gl.h>
#include <cstddef>
#include <string>
#include <memory>

class Model {
public:
    enum class VertexLayout {
        Pos3_Norm3,         // 6 floats (position: loc 0, normal: loc 1 & 2)
        Pos3_Col3_Norm3,    // 9 floats (position: loc 0, color: loc 1, normal: loc 2)
        Pos3_Norm3_Col3,    // 9 floats (position: loc 0, normal: loc 1, color: loc 2)
        Pos3_Norm3_UV2,     // 8 floats (position: loc 0, normal: loc 1, uv: loc 2)
        Pos3_Only           // 3 floats (position: loc 0)
    };

    Model(const float* vertices, std::size_t byteSize);
    Model(const float* vertices, std::size_t byteSize, bool hasNormals); // pro 9f
    Model(const float* vertices, std::size_t byteSize, bool hasNormals, bool normalBeforeColor);
    Model(const float* vertices, std::size_t byteSize, VertexLayout layout);
    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void draw() const;

    static std::unique_ptr<Model> fromOBJ(const std::string& filePath);

private:
    void initBuffers(const float* vertices, std::size_t byteSize, VertexLayout layout);

    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
};
