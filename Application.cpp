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

    // Modely
    bushModel = std::make_unique<Model>(::bushes, sizeof(::bushes));
    treeModel = std::make_unique<Model>(::tree, sizeof(::tree));
	sphereModel = std::make_unique<Model>(::sphere, sizeof(::sphere));
	triangleModel = std::make_unique<Model>(::triangle, sizeof(::triangle));
	loginModel = std::make_unique<Model>(::login, sizeof(::login));

    sunModel = std::make_unique<Model>(::sun, sizeof(::sun), true);
    earthModel = std::make_unique<Model>(::earth, sizeof(::earth), true);
    moonModel = std::make_unique<Model>(::moon, sizeof(::moon), true);

	groundModel = std::make_unique<Model>(::ground, sizeof(::ground));

    clockModel = std::make_unique<Model>(::clockModel, sizeof(::clockModel), true, true);

    // new
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), static_cast<float>(width) / static_cast<float>(height));

    basicShader = std::make_unique<ShaderProgram>("shaders/basic.vert", "shaders/basic.frag");
    treeShader = std::make_unique<ShaderProgram>("shaders/tree.vert", "shaders/basic.frag");
    clockShader = std::make_unique<ShaderProgram>("shaders/clock.vert", "shaders/clock.frag");
    ambientShader = std::make_unique<ShaderProgram>("shaders/phong.vert", "shaders/ambient.frag");
    lambertShader = std::make_unique<ShaderProgram>("shaders/phong.vert", "shaders/lambert.frag");
    phongShader = std::make_unique<ShaderProgram>("shaders/phong.vert", "shaders/phong.frag");
    blinnShader = std::make_unique<ShaderProgram>("shaders/phong.vert", "shaders/bphong.frag");


    basicShader->setCamera(camera.get());
    treeShader->setCamera(camera.get());
    clockShader->setCamera(camera.get());
    phongShader->setCamera(camera.get());
    lambertShader->setCamera(camera.get());
    blinnShader->setCamera(camera.get());
	ambientShader->setCamera(camera.get());


	//phongShader->setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    scenes.push_back(SceneFactory::createTriangleScene(*basicShader, *triangleModel));
    scenes.push_back(SceneFactory::createSphereScene(*basicShader, *sphereModel));
    scenes.push_back(SceneFactory::createForestScene(*basicShader, *treeShader, *clockShader, *treeModel, *bushModel, *sphereModel, *clockModel, *groundModel));
    scenes.push_back(SceneFactory::createLoginScene(*basicShader, *loginModel));
    scenes.push_back(SceneFactory::createSolarSystemScene(*basicShader,*phongShader, *sunModel, *earthModel, *moonModel));
	scenes.push_back(SceneFactory::createLightScene(*blinnShader, *sphereModel));



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
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            currentSceneIndex = 4;
        }
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
			currentSceneIndex = 5;
		}

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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