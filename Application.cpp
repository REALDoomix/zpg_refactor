#include "Application.h"
#include "Scene.h"
#include "Models/bushes.h"
#include "Models/vsbLogin.h"
#include "Models/tree.h"
#include "Models/sphere.h"
#include "Models/triangle.h"
#include <glm/vec2.hpp>

#include <iostream>
#include <stdexcept>

Application::Application() = default;

Application::~Application() {
    scenes.clear();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization() {
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

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("GLAD initialization failed");
    }

    glEnable(GL_DEPTH_TEST);

    // Modely
    bushModel = std::make_unique<Model>(::bushes, sizeof(::bushes));
    treeModel = std::make_unique<Model>(::tree, sizeof(::tree));
	sphereModel = std::make_unique<Model>(::sphere, sizeof(::sphere));
	triangleModel = std::make_unique<Model>(::triangle, sizeof(::triangle));
	loginModel = std::make_unique<Model>(::login, sizeof(::login));

    // Scéna 1
    scenes.push_back(std::make_unique<Scene>());
    scenes[0]->addDrawable(*triangleModel).setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Scéna 2
    scenes.push_back(std::make_unique<Scene>());
    scenes[1]->addDrawable(*sphereModel).setPosition(glm::vec3(-0.5f, 0.5f, 0.0f));
	scenes[1]->getDrawables()[0].setScale(0.1f);

    // Scéna 3
    scenes.push_back(std::make_unique<Scene>());
    scenes[2]->addDrawable(*treeModel).setPosition(glm::vec3(8.0f, -8.0f, 0.5f));
    scenes[2]->getDrawables()[0].setScale(0.1f);
    for (size_t i = 0; i < 11; i++)
    {
		scenes[2]->addDrawable(*treeModel).setPosition(glm::vec3(-7.5f + i*1.6f, -1.0f, 0.5f));
		scenes[2]->getDrawables()[i+1].setScale(0.1f);
    }
    for (size_t i = 0; i < 11; i++)
    {
        scenes[2]->addDrawable(*bushModel).setPosition(glm::vec3(-2.5f + i * 0.5f, -1.0f, 0.5f));
        scenes[2]->getDrawables()[i+12].setScale(0.2f);
    }

	scenes[2]->addDrawable(*sphereModel).setScale(0.2f);
    scenes[2]->getDrawables()[23].setPosition(glm::vec3(4.0f, 4.0f, 0.5f));
	scenes[2]->getDrawables()[23].setColor(glm::vec3(1.0f, 1.0f, 0.0f));

	scenes.push_back(std::make_unique<Scene>());
    scenes[3]->addDrawable(*loginModel).setScale(0.4f);
}

void Application::run() {
    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Přepínání scén
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            currentSceneIndex = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            currentSceneIndex = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            currentSceneIndex = 2;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            currentSceneIndex = 3;
        }

        scenes[currentSceneIndex]->draw();
        if (currentSceneIndex == 2) {
			scenes[2]->getDrawables()[0].addAngle(50.0f * deltaTime);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::error_callback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << "\n";
}

void Application::key_callback(GLFWwindow* window, int key, int, int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Application::window_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}