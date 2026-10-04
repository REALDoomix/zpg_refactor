#pragma once

#include <glm/vec2.hpp>

class Transformation {
public:
    Transformation();

    // Rotace pomocí rovnic
    static glm::vec2 rotate(glm::vec2 point, float angleDegrees);

    // Gettery pro úhel a pozici
    float getAngle() const { return angle; }
    glm::vec2 getPosition() const { return position; }

    // Settery
    void setAngle(float newAngle) { angle = newAngle; }
    void setPosition(glm::vec2 newPosition) { position = newPosition; }
    void addAngle(float deltaAngle) { angle += deltaAngle; }
	void addPosition(glm::vec2 deltaPosition) { position += deltaPosition; }

private:
    float angle = 0.0f;
    glm::vec2 position = glm::vec2(0.0f);
};