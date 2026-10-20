#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Camera.h"

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

    // Modely
    std::unique_ptr<Model> bushModel;
    std::unique_ptr<Model> treeModel;
    std::unique_ptr<Model> sphereModel;
	std::unique_ptr<Model> triangleModel;
	std::unique_ptr<Model> loginModel;
    std::unique_ptr<Model> groundModel;

    std::unique_ptr<Model> sunModel;
    std::unique_ptr<Model> earthModel;
    std::unique_ptr<Model> moonModel;

	std::unique_ptr<Model> clockModel;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

	std::unique_ptr<Camera> camera;

    std::unique_ptr<ShaderProgram> basicShader;
    std::unique_ptr<ShaderProgram> treeShader;
    std::unique_ptr<ShaderProgram> clockShader;



    int width = 800;
    int height = 600;
    float rotationSpeed = 5.0f;
    float movementSpeed = 0.05f;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};