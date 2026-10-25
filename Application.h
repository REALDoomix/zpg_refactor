#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Camera.h"
#include "ModelManager.h"
#include "ShaderManager.h"

class Scene;
class Model;
class ShaderProgram;

class Application {
public:
    Application();
    ~Application();

    void initialization();
    void run();

private:
    GLFWwindow* window = nullptr;
    std::vector<std::unique_ptr<Scene>> scenes;
    int currentSceneIndex = 0;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;

	ModelManager modelManager;
	ShaderManager shaderManager;


    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

	std::unique_ptr<Camera> camera;


    int width = 800;
    int height = 600;
    float rotationSpeed = 5.0f;
    float movementSpeed = 0.05f;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};