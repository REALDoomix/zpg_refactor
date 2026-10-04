#include "Application.h"
#include "Scene.h"

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
    scenes.push_back(std::make_unique<Scene>());
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Ovládání šipkami
        //rotace
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            scene->getTree().getTransformation().addAngle(rotationSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            scene->getTree().getTransformation().addAngle(-rotationSpeed);
        }
        // posun
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			scene->getTree().getTransformation().addPosition(glm::vec2(0.0f, movementSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			scene->getTree().getTransformation().addPosition(glm::vec2(0.0f, -movementSpeed));
		}
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            scene->getTree().getTransformation().addPosition(glm::vec2(-movementSpeed, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            scene->getTree().getTransformation().addPosition(glm::vec2(movementSpeed, 0.0f));
        }

        // scene switching
        /*if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            scene = std::make_unique<Scene>();
        }*/

        scene->draw();
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