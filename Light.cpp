#include "Light.h"

Light::Light(const glm::vec3& staticPos, const glm::vec3& col)
    : staticPosition(staticPos), color(col), attachedTransform(nullptr) {
}

Light::Light(std::shared_ptr<ITransformation> transform, const glm::vec3& col)
    : color(col), attachedTransform(transform) {
}

glm::vec3 Light::getPosition() const {
    if (attachedTransform) {
        glm::mat4 matrix = attachedTransform->getMatrix();
        // 4. sloupec matice [3] obsahuje translaci ve world-space
        return glm::vec3(matrix[3]);
    }
    return staticPosition;
}

void Light::setStaticPosition(const glm::vec3& pos) {
    staticPosition = pos;
    attachedTransform = nullptr;
}

void Light::attachTo(std::shared_ptr<ITransformation> transform) {
    attachedTransform = transform;
}
