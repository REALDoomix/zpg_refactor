#include "Application.h"
#include "Scene.h"

//modely
#include "Models/bushes.h"
#include "Models/vsbLogin.h"
#include "Models/tree.h"
#include "Models/sphere.h"
#include "Models/triangle.h"
#include "Models/earth.h"
#include "Models/moon.h"
#include "Models/sun.h"


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

    // Scéna 1 — demonstrace pořadí transformací (translate vs rotate)
    scenes.push_back(std::make_unique<Scene>());

    // Translate -> Rotate (T * R)
    auto compositeA = std::make_unique<TransformationComposite>();
    compositeA->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.5f, 0.0f, 0.0f)));
    compositeA->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    scenes[0]->addDrawable(*triangleModel).setTransformation(std::move(compositeA)).setColor(glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotate -> Translate (R * T)
    auto compositeB = std::make_unique<TransformationComposite>();
    compositeB->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    compositeB->add(std::make_unique<TranslateTransformation>(glm::vec3(1.5f, 0.0f, 0.0f)));
    scenes[0]->addDrawable(*triangleModel).setTransformation(std::move(compositeB)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));

    scenes[0]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, -1.7f, 0.0f), 0.4f));

    // Scéna 2 koule
    scenes.push_back(std::make_unique<Scene>());
    scenes[1]->addDrawable(*sphereModel)
        .setTransformation(makeStaticTransform(glm::vec3(-0.5f, 0.5f, 0.0f), 0.1f));
    scenes[1]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, -1.7f, 0.0f), 0.4f));

    // Scéna 3 les 
    scenes.push_back(std::make_unique<Scene>());

    for (int i = 0; i < 11; i++) {
        scenes[2]->addDrawable(*treeModel)
            .setTransformation(makeRandomTransform(-5.0f, 5.0f, -1.0f, -6.0f, 2.0f, 0.08f, 0.14f));
    }

    for (int i = 0; i < 11; i++) {
        scenes[2]->addDrawable(*bushModel)
            .setTransformation(makeRandomTransform(-4.0f, 4.0f, -1.0f, -4.0f, 1.0f, 0.4f, 0.6f));
    }

    scenes[2]->addDrawable(*sphereModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, 1.8f, 0.5f), 0.2f))
        .setColor(glm::vec3(1.0f, 1.0f, 0.0f));
    scenes[2]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, -1.7f, 0.0f), 0.4f));

    // Scéna 4 login
    scenes.push_back(std::make_unique<Scene>());
    scenes[3]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f));
    scenes[3]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, -1.7f, 0.0f), 0.4f));


    // Scéna 5 — sluneční soustava
    scenes.push_back(std::make_unique<Scene>());
   
    constexpr float earthOrbitSpeed = 20.0f;   // deg/sec around Sun
    constexpr float earthOrbitRadius = 2.0f;
    constexpr float earthSpinSpeed = 90.0f;    // deg/sec, own axis
    constexpr float earthScale = 0.3f;

    constexpr float moonOrbitSpeed = 80.0f;    // deg/sec around Earth
    constexpr float moonOrbitRadius = 0.8f;
    constexpr float moonSpinSpeed = 40.0f;
    constexpr float moonScale = 0.1f;

    // Sun — stays at origin, only spins on its own axis
    auto sunComposite = std::make_unique<TransformationComposite>();
    sunComposite->add(std::make_unique<DynamicRotateTransformation>(10.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    sunComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(1.0f)));
    scenes[4]->addDrawable(*sunModel)
        .setTransformation(std::move(sunComposite));

    // Earth — orbits Sun, spins on its own axis
    auto earthComposite = std::make_unique<TransformationComposite>();
    addOrbit(*earthComposite, earthOrbitSpeed, earthOrbitRadius);
    earthComposite->add(std::make_unique<DynamicRotateTransformation>(earthSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    earthComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(earthScale)));
    scenes[4]->addDrawable(*earthModel)
        .setTransformation(std::move(earthComposite));

    // Moon — follows Earth's orbital path AROUND the Sun, then orbits Earth on top of that, then spins
    auto moonComposite = std::make_unique<TransformationComposite>();
    addOrbit(*moonComposite, earthOrbitSpeed, earthOrbitRadius);  // same params as Earth's orbit — keeps Moon traveling with Earth
    addOrbit(*moonComposite, moonOrbitSpeed, moonOrbitRadius);    // Moon's own orbit around Earth
    moonComposite->add(std::make_unique<DynamicRotateTransformation>(moonSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(moonScale)));
    scenes[4]->addDrawable(*moonModel)
        .setTransformation(std::move(moonComposite));

    scenes[4]->addDrawable(*loginModel)
        .setTransformation(makeStaticTransform(glm::vec3(2.0f, -1.7f, 0.0f), 0.4f));


    // new
    viewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),   // eye position
        glm::vec3(0.0f, 0.0f, 0.0f),   // look-at target
        glm::vec3(0.0f, 1.0f, 0.0f)    // up vector
    );

    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f, 100.0f
    );
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

        scenes[currentSceneIndex]->update(deltaTime);
        scenes[currentSceneIndex]->draw(viewMatrix, projectionMatrix);
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