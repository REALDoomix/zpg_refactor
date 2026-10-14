#include "SceneFactory.h"
#include "../Transformations.h"

std::unique_ptr<Scene> SceneFactory::createTriangleScene(ShaderProgram& shader, Model& triangleModel) {
    auto scene = std::make_unique<Scene>();

    auto compositeA = std::make_unique<TransformationComposite>();
    compositeA->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.5f, 0.0f, 0.0f)));
    compositeA->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    scene->addDrawable(triangleModel,shader).setTransformation(std::move(compositeA));

    auto compositeB = std::make_unique<TransformationComposite>();
    compositeB->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    compositeB->add(std::make_unique<TranslateTransformation>(glm::vec3(1.5f, 0.0f, 0.0f)));
    scene->addDrawable(triangleModel,shader).setTransformation(std::move(compositeB));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSphereScene(ShaderProgram& shader, Model& sphereModel) {
    auto scene = std::make_unique<Scene>();
    scene->addDrawable(sphereModel,shader)
        .setTransformation(makeStaticTransform(glm::vec3(-0.5f, 0.5f, 0.0f), 0.1f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createForestScene(ShaderProgram& basicShader, ShaderProgram& treeShader, Model& treeModel, Model& bushModel, Model& sphereModel) {
    auto scene = std::make_unique<Scene>();

    auto composite = std::make_unique<TransformationComposite>();
    composite->add(std::make_unique<TranslateTransformation>(glm::vec3(8.0f, -8.0f, 0.5f)));
    composite->add(std::make_unique<DynamicRotateTransformation>(50.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    composite->add(std::make_unique<ScaleTransformation>(glm::vec3(0.1f)));
    scene->addDrawable(treeModel, treeShader).setTransformation(std::move(composite));

    constexpr int treeCount = 74;
    for (int i = 0; i < treeCount; i++) {
        scene->addDrawable(treeModel, treeShader)
            .setTransformation(makeRandomTransform(-8.0f, 8.0f, -1.0f, -6.0f, 2.0f, 0.08f, 0.14f));
    }

    constexpr int bushCount = 80;
    for (int i = 0; i < bushCount; i++) {
        scene->addDrawable(bushModel, basicShader)
            .setTransformation(makeRandomTransform(-5.0f, 5.0f, -1.0f, -4.0f, 1.0f, 0.4f, 0.6f)).setColor(glm::vec3(0.0f, 0.5f, 0.0f));
    }

    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(glm::vec3(4.0f, 4.0f, 0.5f), 0.2f))
        .setColor(glm::vec3(1.0f, 1.0f, 0.0f));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createLoginScene(ShaderProgram& shader, Model& loginModel) {
    auto scene = std::make_unique<Scene>();
    scene->addDrawable(loginModel,shader)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSolarSystemScene(ShaderProgram& shader, Model& sunModel, Model& earthModel, Model& moonModel) {
    auto scene = std::make_unique<Scene>();

    constexpr float earthOrbitSpeed = 20.0f, earthOrbitRadius = 4.0f, earthSpinSpeed = 90.0f, earthScale = 0.3f;
    constexpr float moonOrbitSpeed = 80.0f, moonOrbitRadius = 0.8f, moonSpinSpeed = 40.0f, moonScale = 0.1f;

    auto sunComposite = std::make_unique<TransformationComposite>();
    sunComposite->add(std::make_unique<DynamicRotateTransformation>(10.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    sunComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(1.0f)));
    scene->addDrawable(sunModel,shader).setTransformation(std::move(sunComposite));

    auto earthComposite = std::make_unique<TransformationComposite>();
    addOrbit(*earthComposite, earthOrbitSpeed, earthOrbitRadius);
    earthComposite->add(std::make_unique<DynamicRotateTransformation>(earthSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    earthComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(earthScale)));
    scene->addDrawable(earthModel,shader).setTransformation(std::move(earthComposite));

    auto moonComposite = std::make_unique<TransformationComposite>();
    addOrbit(*moonComposite, earthOrbitSpeed, earthOrbitRadius);
    addOrbit(*moonComposite, moonOrbitSpeed, moonOrbitRadius);
    moonComposite->add(std::make_unique<DynamicRotateTransformation>(moonSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(moonScale)));
    scene->addDrawable(moonModel,shader).setTransformation(std::move(moonComposite));

    return scene;
}