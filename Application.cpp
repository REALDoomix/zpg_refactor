#include "Application.h"
#include "Scene.h"
#include "ShaderProgram.h"

#include "./Scenes/SceneFactory.h"

//modely
#include "Models/bushes.h"
#include "Models/vsbLogin.h"
#include "Models/tree.h"
#include "Models/sphere.h"
#include "Models/triangle.h"
#include "Models/earth.h"
#include "Models/moon.h"
#include "Models/astronaut.h"
#include "Models/sun.h"
#include "Models/clock.h"
#include "Models/ground.h"


#include <glm/vec2.hpp>

#include <cstdlib>
#include <ctime>

#include <iostream>
#include <stdexcept>

Application::Application() = default;

Application::~Application() {
    scenes.clear();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization() {
    //start randomizer
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Unable to initialize GLFW");
    }

    window = glfwCreateWindow(width, height, "ZPG", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Unable to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, window_size_callback);


    // myš

	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("GLAD initialization failed");
    }

    glEnable(GL_DEPTH_TEST);

    modelManager.load("bush", ::bushes, sizeof(::bushes));
    modelManager.load("tree", ::tree, sizeof(::tree));
    modelManager.load("sphere", ::sphere, sizeof(::sphere));
    modelManager.load("triangle", ::triangle, sizeof(::triangle));
    modelManager.load("login", ::login, sizeof(::login));
    modelManager.load("ground", ::ground, sizeof(::ground));

    modelManager.load("sun", ::sun, sizeof(::sun), true);
    modelManager.load("earth", ::earth, sizeof(::earth), true);
    modelManager.load("moon", ::moon, sizeof(::moon), true);
    modelManager.load("astronaut", ::astronaut, sizeof(::astronaut), true);

    modelManager.load("clock", ::clockModel, sizeof(::clockModel), true, true);


    // new
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 4.0f), static_cast<float>(width) / static_cast<float>(height));

    shaderManager.load("basic", "shaders/basic.vert", "shaders/basic.frag");
    shaderManager.load("tree", "shaders/tree.vert", "shaders/basic.frag");
    shaderManager.load("clock", "shaders/clock.vert", "shaders/clock.frag");
    shaderManager.load("ambient", "shaders/phong.vert", "shaders/ambient.frag");
    shaderManager.load("lambert", "shaders/phong.vert", "shaders/lambert.frag");
    shaderManager.load("phong", "shaders/phong.vert", "shaders/phong.frag");
    shaderManager.load("blinn", "shaders/phong.vert", "shaders/bphong.frag");

    shaderManager.setCamera(camera.get());

    

    scenes.push_back(SceneFactory::createTriangleScene(shaderManager, modelManager));
    scenes.push_back(SceneFactory::createSphereScene(shaderManager, modelManager));
    scenes.push_back(SceneFactory::createForestScene(shaderManager, modelManager));
    scenes.push_back(SceneFactory::createLoginScene(shaderManager, modelManager));
    scenes.push_back(SceneFactory::createSolarSystemScene(shaderManager, modelManager));
    scenes.push_back(SceneFactory::createLightScene(shaderManager, modelManager));

}

void Application::run() {
    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			// MOVE FORWARD
			camera->processKeyboard(Camera::Movement::FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // MOVE BACKWARD
            camera->processKeyboard(Camera::Movement::BACKWARD, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // MOVE LEFT
            camera->processKeyboard(Camera::Movement::LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // MOVE RIGHT
            camera->processKeyboard(Camera::Movement::RIGHT, deltaTime);
        }

        scenes[currentSceneIndex]->update(deltaTime);
        scenes[currentSceneIndex]->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::error_callback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << "\n";
}

void Application::key_callback(GLFWwindow* window, int key, int, int action, int) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_1) app->currentSceneIndex = 0;
        if (key == GLFW_KEY_2) app->currentSceneIndex = 1;
        if (key == GLFW_KEY_3) app->currentSceneIndex = 2;
        if (key == GLFW_KEY_4) app->currentSceneIndex = 3;
        if (key == GLFW_KEY_5) app->currentSceneIndex = 4;
        if (key == GLFW_KEY_6) app->currentSceneIndex = 5;
    }
}

void Application::window_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);

	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));

	if (app && app->camera && height > 0) {
		app->camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
	}
}

void Application::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || !app->camera) return;

    if (app->firstMouse) {
        app->lastMouseX = xpos;
        app->lastMouseY = ypos;
        app->firstMouse = false;
    }

    float xOffset = static_cast<float>(xpos - app->lastMouseX);
    float yOffset = static_cast<float>(ypos - app->lastMouseY);

    app->lastMouseX = xpos;
    app->lastMouseY = ypos;
	if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		app->camera->processMouse(xOffset, yOffset);
	}
   
}