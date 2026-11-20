#include "Application.h"
#include "Scene.h"
#include "ShaderProgram.h"

#include "./Scenes/SceneFactory.h"

// modely
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
#include "Models/plain.h"

#include <glm/vec2.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <glad/gl.h>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

Application::Application() = default;

Application::~Application() {
    scenes.clear();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization() {
    // start randomizer
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Unable to initialize GLFW");
    }

    window = glfwCreateWindow(width, height, "ZPG - Ales Najser - NAJ0062", nullptr, nullptr);
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

    TracyGpuContext;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Test na maximální počet najednou použitelných texturovacích jednotek (podle zadání cv.)
    int maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    printf("GL_MAX_TEXTURE_IMAGE_UNITS: %d\n", maxTextureUnits);

    modelManager.load("bush", ::bushes, sizeof(::bushes));
    modelManager.load("tree", ::tree, sizeof(::tree));
    modelManager.load("sphere", ::sphere, sizeof(::sphere));
    modelManager.load("triangle", ::triangle, sizeof(::triangle));
    modelManager.load("login", ::login, sizeof(::login));
    modelManager.load("ground", ::ground, sizeof(::ground));
    modelManager.loadWithUV("plain", ::plain, sizeof(::plain));

    modelManager.load("sun", ::sun, sizeof(::sun), true);
    modelManager.load("earth", ::earth, sizeof(::earth), true);
    modelManager.load("moon", ::moon, sizeof(::moon), true);
    modelManager.load("astronaut", ::astronaut, sizeof(::astronaut), true);

    modelManager.load("clock", ::clockModel, sizeof(::clockModel), true, true);

    // Načtení OBJ modelů s UV souřadnicemi
    modelManager.loadOBJ("shrek", "Models/shrek.obj");
    modelManager.loadOBJ("toilet", "Models/toiled.obj");
    modelManager.loadOBJ("fiona", "Models/fiona.obj");
    modelManager.loadOBJ("skybox", "Models/skybox.obj");

    // Načtení OBJ modelů učebny EB408
    modelManager.loadOBJ("EB408", "Models/EB408.obj");
    modelManager.loadOBJ("Door", "Models/Door.obj");
    modelManager.loadOBJ("Whiteboard", "Models/Whiteboard.obj");
    modelManager.loadOBJ("table", "Models/table.obj");
    modelManager.loadOBJ("OrangeChair", "Models/OrangeChair.obj");

    // Načtení textur pomocí stb_image
    textureManager.load("wooden_fence", "Models/Textures/wooden_fence.png");
    textureManager.load("shrek", "Models/Textures/shrek.png");
    textureManager.load("toilet", "Models/Textures/toiled.jpg");
    textureManager.load("fiona", "Models/Textures/fiona.png");

    // Textury učebny EB408
    textureManager.load("EB408", "Models/Textures/T_EB408.png");
    textureManager.load("Door", "Models/Textures/T_door_basecolor.png");
    textureManager.load("Whiteboard", "Models/Textures/T_Whiteboard_BaseColor.png");
    textureManager.load("table", "Models/Textures/T_Table_BaseColor.png");
    textureManager.load("OrangeChair", "Models/Textures/T_OrangeChair_BaseColor.png");

    std::vector<std::string> skyboxFaces = {
        "Models/Textures/skybox/posx.jpg",
        "Models/Textures/skybox/negx.jpg",
        "Models/Textures/skybox/posy.jpg",
        "Models/Textures/skybox/negy.jpg",
        "Models/Textures/skybox/posz.jpg",
        "Models/Textures/skybox/negz.jpg"
    };
    textureManager.loadCubeMap("skybox", skyboxFaces);

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 4.0f), static_cast<float>(width) / static_cast<float>(height));

    shaderManager.load("basic", "shaders/Vertex/basic.vert", "shaders/Fragment/basic.frag");
    shaderManager.load("tree", "shaders/Vertex/tree.vert", "shaders/Fragment/basic.frag");
    shaderManager.load("clock", "shaders/Vertex/clock.vert", "shaders/Fragment/clock.frag");
    shaderManager.load("ambient", "shaders/Vertex/phong.vert", "shaders/Fragment/ambient.frag");
    shaderManager.load("lambert", "shaders/Vertex/phong.vert", "shaders/Fragment/lambert.frag");
    shaderManager.load("phong", "shaders/Vertex/phong.vert", "shaders/Fragment/phong.frag");
    shaderManager.load("blinn", "shaders/Vertex/phong.vert", "shaders/Fragment/bphong.frag");
    shaderManager.load("light", "shaders/Vertex/phong.vert", "shaders/Fragment/newPhong.frag");
    shaderManager.load("bad_normal", "shaders/Vertex/phong_bad_normal.vert", "shaders/Fragment/phong.frag");
    shaderManager.load("phong_bleed", "shaders/Vertex/phong.vert", "shaders/Fragment/phong_bleed.frag");
    shaderManager.load("phong_clipped", "shaders/Vertex/phong.vert", "shaders/Fragment/phong_clipped.frag");
    shaderManager.load("night_tree", "shaders/Vertex/night_tree.vert", "shaders/Fragment/night_forest.frag");
    shaderManager.load("night_forest", "shaders/Vertex/phong.vert", "shaders/Fragment/night_forest.frag");

    // Nové shadery pro textury, multi-texturing a skybox
    shaderManager.load("texture", "shaders/Vertex/texture.vert", "shaders/Fragment/texture.frag");
    shaderManager.load("multi_texture", "shaders/Vertex/texture.vert", "shaders/Fragment/multi_texture.frag");
    shaderManager.load("texture_phong", "shaders/Vertex/texture.vert", "shaders/Fragment/texture_phong.frag");
    shaderManager.load("skybox", "shaders/Vertex/skybox.vert", "shaders/Fragment/skybox.frag");
    // Classroom shader with balanced lighting
    shaderManager.load("classroom", "shaders/Vertex/texture.vert", "shaders/Fragment/classroom.frag");

    shaderManager.setCamera(camera.get());

    // Inicializace reflektoru (baterky svázané s kamerou)
    flashlight = std::make_shared<Light>();
    flashlight->type = LightType::SPOTLIGHT;
    flashlight->color = glm::vec3(1.0f, 0.98f, 0.90f);
    flashlight->constant = 1.0f;
    flashlight->linear = 0.08f;
    flashlight->quadratic = 0.025f;
    flashlight->spotCutoff = 0.985f; // mezní úhel kuželu reflektoru (cca 10 stupňů)
    flashlight->enabled = flashlightEnabled;

    scenes.push_back(SceneFactory::createTriangleScene(shaderManager, modelManager));          // 1
    scenes.push_back(SceneFactory::createSphereScene(shaderManager, modelManager));            // 2 (Více světel + dynamické světlo)
    scenes.push_back(SceneFactory::createForestScene(shaderManager, modelManager));            // 3 (Denní les)
    scenes.push_back(SceneFactory::createLoginScene(shaderManager, modelManager));             // 4
    scenes.push_back(SceneFactory::createSolarSystemScene(shaderManager, modelManager));        // 5
    scenes.push_back(SceneFactory::createLightScene(shaderManager, modelManager));             // 6 (4 koule - Ambient, Lambert, Phong, Blinn)
    scenes.push_back(SceneFactory::createNormalDemoScene(shaderManager, modelManager));        // 7 (Demonstrace chybného vs správného převodu normál)
    scenes.push_back(SceneFactory::createSpecularBleedDemoScene(shaderManager, modelManager)); // 8 (Demonstrace přesvitu na odvrácené straně s mocninou 1 vs ořezání)
    scenes.push_back(SceneFactory::createNightForestScene(shaderManager, modelManager, flashlight)); // 9 (Noční les se světluškami a baterkou)
    scenes.push_back(SceneFactory::createTextureDemoScene(shaderManager, modelManager, textureManager));  // 0 (Jedna textura na plain)
    scenes.push_back(SceneFactory::createMultiTextureScene(shaderManager, modelManager, textureManager)); // T (Více textur na jednom modelu)
    scenes.push_back(SceneFactory::createShrekScene(shaderManager, modelManager, textureManager));        // Y (Shrek a toaleta s texturami a osvětlením)
    scenes.push_back(SceneFactory::createSkyboxScene(shaderManager, modelManager, textureManager));       // U (Skybox s cubemapou a objekty)
    scenes.push_back(SceneFactory::createEB408Scene(shaderManager, modelManager, textureManager));       // I (Učebna EB408 s tabulí, nábytkem a světly)

    std::cout << "=========================NAJ0062============================\n";
    std::cout << "  Ovladani klavesami 1 - 9, 0, T, Y, U, I:\n";
    std::cout << " 1: Triangle\n";
    std::cout << " 2: Pole svetel (cervene, modre + obihajici zlute)\n";
    std::cout << " 3: Denni les\n";
    std::cout << " 4: VSB Login\n";
    std::cout << " 5: Slunecni soustava\n";
    std::cout << " 6: 4 modely osvetleni\n";
    std::cout << " 7: Demonstrace prevodu normal (chybny vlevo vs spravny vpravo)\n";
    std::cout << " 8: Presvit zrcadlove slozky (chybny vlevo vs orezany vpravo)\n";
    std::cout << " 9: Nocni les (bodova svetla - svetlusky s nahodnym pohybem + baterka)\n";
    std::cout << " 0: Jedna textura (Plain s texturou dreveneho plotu)\n";
    std::cout << " T: Vice textur na jednom modelu (Multi-texturing)\n";
    std::cout << " Y: Shrek a toaleta (Ruzne modely s ruznymi texturami a Phongem)\n";
    std::cout << " U: Skybox (Seamless cubemap kostka)\n";
    std::cout << " I: Ucebna EB408 (Model ucebny, dvere, tabule se ZPG vzorci, stoly, zidle, osvetleni)\n";
    std::cout << " F: Zapnout / vypnout baterku (reflektor svitici ve smeru kamery)\n";
    std::cout << " P: Prepnout zpusob resize okna (aktualizace perspektivy a viewportu)\n";
    std::cout << "============================================================\n";
}

void Application::run() {
    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        ZoneScopedN("Frame");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

       
         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
             camera->processKeyboard(Camera::Movement::FORWARD, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
             camera->processKeyboard(Camera::Movement::BACKWARD, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
             camera->processKeyboard(Camera::Movement::LEFT, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
             camera->processKeyboard(Camera::Movement::RIGHT, deltaTime);
         }
        
        // Aktualizace pozice a směru baterky podle kamery
        if (flashlight) {
            flashlight->setStaticPosition(camera->getPos());
            flashlight->direction = camera->getFront();
            flashlight->enabled = flashlightEnabled;
        }

        {
            ZoneScopedN("SceneUpdate");
            scenes[currentSceneIndex]->update(deltaTime);
        }

        {
            TracyGpuZone("SceneDrawGPU"); // <-- Measures GPU time for the active scene
            scenes[currentSceneIndex]->draw();
        }


        scenes[currentSceneIndex]->update(deltaTime);
        scenes[currentSceneIndex]->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();

        TracyGpuCollect; // <-- Collects GPU timestamps from OpenGL
        FrameMark;       // <-- Tells Tracy a frame just finished
    }    
}

void Application::error_callback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << "\n";
}

void Application::key_callback(GLFWwindow* window, int key, int, int action, int) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

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
        if (key == GLFW_KEY_7) app->currentSceneIndex = 6;
        if (key == GLFW_KEY_8) app->currentSceneIndex = 7;
        if (key == GLFW_KEY_9) app->currentSceneIndex = 8;
        if (key == GLFW_KEY_0) app->currentSceneIndex = 9;
        if (key == GLFW_KEY_T) app->currentSceneIndex = 10;
        if (key == GLFW_KEY_Y) app->currentSceneIndex = 11;
        if (key == GLFW_KEY_U) app->currentSceneIndex = 12;
        if (key == GLFW_KEY_I) {
            app->currentSceneIndex = 13;
            if (app->camera) {
                app->camera->setPosition(glm::vec3(0.0f, 1.7f, 6.5f));
            }
            std::cout << "[SCENA]: Prepnuto na ucebnu EB408. Stisknete 'C' pro filmovy prulet.\n";
        }

        if (key == GLFW_KEY_F) {
            app->flashlightEnabled = !app->flashlightEnabled;
            if (app->flashlight) {
                app->flashlight->enabled = app->flashlightEnabled;
            }
            std::cout << "[BATERKA / REFLEKTOR]: "
                      << (app->flashlightEnabled ? "ZAPNUTO" : "VYPNUTO")
                      << " (sviti ve smeru pohledu kamery)\n";
        }

        if (key == GLFW_KEY_P) {
            app->enableCorrectResize = !app->enableCorrectResize;
            std::cout << "[RESIZE MOD] Aktualizace perspektivy: "
                      << (app->enableCorrectResize ? "ZAPNUTO (spravne, zadna deformace)" : "VYPNUTO (chybne, koule se deformuji pri resize)")
                      << std::endl;
        }
    }
}

void Application::window_size_callback(GLFWwindow*, int width, int height) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
    if (!app || height <= 0) return;

    if (app->enableCorrectResize) {
        // SPRÁVNÁ AKTUALIZACE (d):
        // 1. Nastavení viewportu přes celé nové okno
        glViewport(0, 0, width, height);
        // 2. Přepočet poměru stran (aspect ratio) v perspektivní matici kamery
        if (app->camera) {
            app->camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
        }
    } else {
        // CHYBNÉ NASTAVENÍ (c):
        // Viewport se sice roztáhne, ale poměr stran perspektivy zůstane starý -> deformace geometrie!
        glViewport(0, 0, width, height);
        std::cout << "[CHYBA RESIZE]: Poměr stran (aspectRatio) perspektivy NEBYL aktualizován! Zkuste roztáhnout okno.\n";
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
