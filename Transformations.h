#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

class ITransformation {
public:
    virtual ~ITransformation() = default;
    virtual glm::mat4 getMatrix() const = 0;
    virtual void update(float deltaTime) {}
};

class TranslateTransformation : public ITransformation {
    glm::vec3 offset;
public:
    explicit TranslateTransformation(const glm::vec3& offset) : offset(offset) {}
    glm::mat4 getMatrix() const override { return glm::translate(glm::mat4(1.0f), offset); }
};

class RotateTransformation : public ITransformation {
    float angleDegrees;
    glm::vec3 axis;
public:
    RotateTransformation(float angleDegrees, const glm::vec3& axis) : angleDegrees(angleDegrees), axis(axis) {}
    glm::mat4 getMatrix() const override { return glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis); }
};

class ScaleTransformation : public ITransformation {
    glm::vec3 scale;
public:
    explicit ScaleTransformation(const glm::vec3& scale) : scale(scale) {}
    glm::mat4 getMatrix() const override { return glm::scale(glm::mat4(1.0f), scale); }
};

class DynamicRotateTransformation : public ITransformation {
    float currentAngleDegrees = 0.0f;
    float speedDegreesPerSecond;
    glm::vec3 axis;
public:
    DynamicRotateTransformation(float speedDegreesPerSecond, const glm::vec3& axis)
        : speedDegreesPerSecond(speedDegreesPerSecond), axis(axis) {
    }
    void update(float deltaTime) override { currentAngleDegrees += speedDegreesPerSecond * deltaTime; }
    glm::mat4 getMatrix() const override { return glm::rotate(glm::mat4(1.0f), glm::radians(currentAngleDegrees), axis); }
};


// ADD
class TransformationComposite : public ITransformation {
    std::vector<std::unique_ptr<ITransformation>> children;
public:
    void add(std::unique_ptr<ITransformation> transformation) { children.push_back(std::move(transformation)); }
    glm::mat4 getMatrix() const override {
        glm::mat4 result(1.0f);
        for (const auto& child : children) result = result * child->getMatrix();
        return result;
    }
    void update(float deltaTime) override {
        for (auto& child : children) child->update(deltaTime);
    }
};

// Convenience factory position + scale, no rotation case.
inline std::unique_ptr<TransformationComposite> makeStaticTransform(const glm::vec3& position, float scale) {
    auto composite = std::make_unique<TransformationComposite>();
    composite->add(std::make_unique<TranslateTransformation>(position));
    composite->add(std::make_unique<ScaleTransformation>(glm::vec3(scale)));
    return composite;
}
// Takes xmin, xMax, fixed Y position, Z min and Zmax, and Scale mind and max
inline std::unique_ptr<TransformationComposite> makeRandomTransform(
    float xMin, float xMax, float yFixed, float zMin, float zMax,
    float scaleMin, float scaleMax) {

    auto randomFloat = [](float min, float max) {
        return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
        };

    auto composite = std::make_unique<TransformationComposite>();
    composite->add(std::make_unique<TranslateTransformation>(
        glm::vec3(randomFloat(xMin, xMax), yFixed, randomFloat(zMin, zMax))));
    composite->add(std::make_unique<RotateTransformation>(
        randomFloat(0.0f, 360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    composite->add(std::make_unique<ScaleTransformation>(
        glm::vec3(randomFloat(scaleMin, scaleMax))));
    return composite;
}

inline void addOrbit(TransformationComposite& composite, float speedDegreesPerSecond, float radius) {
    composite.add(std::make_unique<DynamicRotateTransformation>(speedDegreesPerSecond, glm::vec3(0.0f, 1.0f, 0.0f)));
    composite.add(std::make_unique<TranslateTransformation>(glm::vec3(radius, 0.0f, 0.0f)));
}