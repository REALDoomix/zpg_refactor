#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Camera.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

class Scene;
class Model;
class ShaderProgram;
class Light;

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
    TextureManager textureManager;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::unique_ptr<Camera> camera;

    int width = 1280;
    int height = 720;
    float rotationSpeed = 5.0f;
    float movementSpeed = 0.05f;

    // Demonstrace (c, d): Správná vs. chybná aktualizace viewportu a perspektivy při změně velikosti okna
    bool enableCorrectResize = true;
    float tourTime = 0.0f;

    // Reflektor (baterka) sledující kameru
    std::shared_ptr<Light> flashlight = nullptr;
    bool flashlightEnabled = false;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};
