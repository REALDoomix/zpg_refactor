#include "Model.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void Model::initBuffers(const float* vertices, std::size_t byteSize, VertexLayout layout) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(byteSize), vertices, GL_STATIC_DRAW);

    switch (layout) {
    case VertexLayout::Pos3_Norm3: {
        vertexCount = static_cast<GLsizei>(byteSize / (6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        break;
    }
    case VertexLayout::Pos3_Col3_Norm3: {
        vertexCount = static_cast<GLsizei>(byteSize / (9 * sizeof(float)));
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1); // color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2); // normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        break;
    }
    case VertexLayout::Pos3_Norm3_Col3: {
        vertexCount = static_cast<GLsizei>(byteSize / (9 * sizeof(float)));
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2); // color
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        break;
    }
    case VertexLayout::Pos3_Norm3_UV2: {
        vertexCount = static_cast<GLsizei>(byteSize / (8 * sizeof(float)));
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        break;
    }
    case VertexLayout::Pos3_Only: {
        vertexCount = static_cast<GLsizei>(byteSize / (3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        break;
    }
    }

    glBindVertexArray(0);
}

Model::Model(const float* vertices, std::size_t byteSize) {
    initBuffers(vertices, byteSize, VertexLayout::Pos3_Norm3);
}

Model::Model(const float* vertices, std::size_t byteSize, bool hasNormals) {
    initBuffers(vertices, byteSize, hasNormals ? VertexLayout::Pos3_Col3_Norm3 : VertexLayout::Pos3_Norm3);
}

Model::Model(const float* vertices, std::size_t byteSize, bool hasNormals, bool normalBeforeColor) {
    if (normalBeforeColor) {
        initBuffers(vertices, byteSize, VertexLayout::Pos3_Norm3_Col3);
    } else {
        initBuffers(vertices, byteSize, hasNormals ? VertexLayout::Pos3_Col3_Norm3 : VertexLayout::Pos3_Norm3);
    }
}

Model::Model(const float* vertices, std::size_t byteSize, VertexLayout layout) {
    initBuffers(vertices, byteSize, layout);
}

Model::~Model() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
}

void Model::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

std::unique_ptr<Model> Model::fromOBJ(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return nullptr;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    std::vector<float> vertexData; // 8 floats per vertex: pos(3), norm(3), uv(2)

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        } else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        } else if (prefix == "vn") {
            glm::vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        } else if (prefix == "f") {
            std::vector<std::string> faceTokens;
            std::string token;
            while (ss >> token) {
                faceTokens.push_back(token);
            }
            if (faceTokens.size() < 3) continue;

            auto parseVertex = [&](const std::string& vStr) {
                int pIdx = 0, tIdx = 0, nIdx = 0;
                size_t slash1 = vStr.find('/');
                if (slash1 == std::string::npos) {
                    pIdx = std::stoi(vStr);
                } else {
                    pIdx = std::stoi(vStr.substr(0, slash1));
                    size_t slash2 = vStr.find('/', slash1 + 1);
                    if (slash2 == std::string::npos) {
                        if (slash1 + 1 < vStr.size()) {
                            tIdx = std::stoi(vStr.substr(slash1 + 1));
                        }
                    } else {
                        if (slash2 > slash1 + 1) {
                            tIdx = std::stoi(vStr.substr(slash1 + 1, slash2 - slash1 - 1));
                        }
                        if (slash2 + 1 < vStr.size()) {
                            nIdx = std::stoi(vStr.substr(slash2 + 1));
                        }
                    }
                }

                glm::vec3 pos(0.0f);
                if (pIdx > 0 && static_cast<size_t>(pIdx) <= temp_positions.size()) {
                    pos = temp_positions[pIdx - 1];
                } else if (pIdx < 0 && static_cast<size_t>(-pIdx) <= temp_positions.size()) {
                    pos = temp_positions[temp_positions.size() + pIdx];
                }

                glm::vec2 uv(0.0f);
                if (tIdx > 0 && static_cast<size_t>(tIdx) <= temp_uvs.size()) {
                    uv = temp_uvs[tIdx - 1];
                } else if (tIdx < 0 && static_cast<size_t>(-tIdx) <= temp_uvs.size()) {
                    uv = temp_uvs[temp_uvs.size() + tIdx];
                }

                glm::vec3 norm(0.0f, 1.0f, 0.0f);
                if (nIdx > 0 && static_cast<size_t>(nIdx) <= temp_normals.size()) {
                    norm = temp_normals[nIdx - 1];
                } else if (nIdx < 0 && static_cast<size_t>(-nIdx) <= temp_normals.size()) {
                    norm = temp_normals[temp_normals.size() + nIdx];
                }

                vertexData.push_back(pos.x);
                vertexData.push_back(pos.y);
                vertexData.push_back(pos.z);
                vertexData.push_back(norm.x);
                vertexData.push_back(norm.y);
                vertexData.push_back(norm.z);
                vertexData.push_back(uv.x);
                vertexData.push_back(uv.y);
            };

            for (size_t i = 2; i < faceTokens.size(); ++i) {
                parseVertex(faceTokens[0]);
                parseVertex(faceTokens[i - 1]);
                parseVertex(faceTokens[i]);
            }
        }
    }

    return std::make_unique<Model>(vertexData.data(), vertexData.size() * sizeof(float), VertexLayout::Pos3_Norm3_UV2);
}
