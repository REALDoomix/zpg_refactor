#pragma once
#include <glm/glm.hpp>

#include <vector>

class IObserver;

class Camera {
public:

	enum class Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

    Camera(const glm::vec3& eye, float aspectRatio);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
	glm::vec3 getPos() const { return eye; }

    void setAspectRatio(float aspectRatio);
    void setPosition(const glm::vec3& newEye, float newAlpha = 1.5707963f, float newFi = -1.5707963f);

	void processMouse(float xOffset, float yOffset);
	void processKeyboard(Movement direction, float deltaTime);

	void attach(IObserver* observer);
	void detach(IObserver* observer);

private:
    glm::vec3 eye;
    glm::vec3 up;
    float alpha;   // vertical angle (from pole), radians
    float fi;      // horizontal angle, radians
    float aspectRatio;

	std::vector<IObserver*> observers;

    glm::vec3 computeTarget() const;

    void notifyObservers();
};