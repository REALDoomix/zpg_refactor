#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

class Scene;
class Model;

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

    // Modely
    std::unique_ptr<Model> bushModel;
    std::unique_ptr<Model> treeModel;
    std::unique_ptr<Model> sphereModel;
	std::unique_ptr<Model> triangleModel;
	std::unique_ptr<Model> loginModel;

    int width = 800;
    int height = 600;
    float rotationSpeed = 5.0f;
    float movementSpeed = 0.05f;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
};