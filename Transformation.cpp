#include "Transformation.h"
#include <glm/glm.hpp>

Transformation::Transformation()
    : angle(0.0f), position(glm::vec2(0.0f)) {
}

glm::vec2 Transformation::rotate(glm::vec2 point, float angleDegrees) {
    float angleRadians = glm::radians(angleDegrees);
    float cosA = glm::cos(angleRadians);
    float sinA = glm::sin(angleRadians);

    // x' = x*cos(α) - y*sin(α)
    // y' = x*sin(α) + y*cos(α)
    float x_new = point.x * cosA - point.y * sinA;
    float y_new = point.x * sinA + point.y * cosA;

    return glm::vec2(x_new, y_new);
}