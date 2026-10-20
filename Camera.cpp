#include "Camera.h"
#include "IObserver.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>


Camera::Camera(const glm::vec3& eye, float aspectRatio)
    : eye(eye), up(0.0f, 1.0f, 0.0f),
    alpha(glm::radians(90.0f)),  // start looking level
    fi(glm::radians(-90.0f)),    // start looking -Z 
    aspectRatio(aspectRatio) {
}

glm::vec3 Camera::computeTarget() const {
    glm::vec3 target;
    target.x = sin(alpha) * cos(fi);
    target.z = sin(alpha) * sin(fi);
    target.y = cos(alpha);
    return target;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(eye, eye + computeTarget(), up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
    notifyObservers();
}

void Camera::processMouse(float xOffset, float yOffset)
{
    constexpr float sensitivity = 0.003f;

    fi += xOffset * sensitivity;
    alpha += yOffset * sensitivity;

    constexpr float epsilon = 0.01f;
    alpha = std::clamp(alpha, epsilon, glm::pi<float>() - epsilon);

	notifyObservers();
}

void Camera::processKeyboard(Movement direction, float deltaTime) {
    constexpr float speed = 3.0f; // units per second

    glm::vec3 front = glm::normalize(computeTarget());
    glm::vec3 right = glm::normalize(glm::cross(front, up));

    float velocity = speed * deltaTime;

    switch (direction) {
    case Movement::FORWARD:  eye += front * velocity; break;
    case Movement::BACKWARD: eye -= front * velocity; break;
    case Movement::LEFT:     eye -= right * velocity; break;
    case Movement::RIGHT:    eye += right * velocity; break;
    }

    notifyObservers();
}

void Camera::attach(IObserver* observer) {
    observers.push_back(observer);
}

void Camera::detach(IObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notifyObservers() {
    for (IObserver* observer : observers) {
        observer->onCameraChanged();
    }
}
