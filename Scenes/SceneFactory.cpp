#include "SceneFactory.h"
#include "../Transformations.h"

std::unique_ptr<Scene> SceneFactory::createTriangleScene(ShaderManager& shaderManager, ModelManager& models) {
    auto scene = std::make_unique<Scene>();
    Model& triangleModel = models.get("triangle");
	ShaderProgram& shader = shaderManager.get("basic");

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

std::unique_ptr<Scene> SceneFactory::createSphereScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& sphereModel = models.get("sphere");

	ShaderProgram& shader = shaderManager.get("basic");

    auto scene = std::make_unique<Scene>();
    scene->addDrawable(sphereModel,shader)
        .setTransformation(makeStaticTransform(glm::vec3(-0.5f, 0.5f, 0.0f), 0.1f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createForestScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& treeModel = models.get("tree");
    Model& bushModel = models.get("bush");
    Model& sphereModel = models.get("sphere");
    Model& clockModel = models.get("clock");
    Model& groundModel = models.get("ground");


	ShaderProgram& basicShader = shaderManager.get("basic");
	ShaderProgram& treeShader = shaderManager.get("tree");
	ShaderProgram& clockShader = shaderManager.get("clock");

    auto scene = std::make_unique<Scene>();

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

    auto clockComposite = std::make_unique<TransformationComposite>();
    clockComposite->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, -0.67f, -2.0f)));
    clockComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(0.2f, 0.2f, 0.2f)));
	clockComposite->add(std::make_unique<RotateTransformation>(-90.0f,(glm::vec3(0.0f, 1.0f, 0.0f))));

    scene->addDrawable(clockModel, clockShader)
        .setTransformation(std::move(clockComposite))
        .enableClockHands();

    auto clockComposite2 = std::make_unique<TransformationComposite>();
    clockComposite2->add(std::make_unique<TranslateTransformation>(glm::vec3(2.3f, -0.67f, -2.0f)));
    clockComposite2->add(std::make_unique<ScaleTransformation>(glm::vec3(0.2f, 0.2f, 0.2f)));
    clockComposite2->add(std::make_unique<RotateTransformation>(90.0f, (glm::vec3(0.0f, 1.0f, 0.0f))));



    scene->addDrawable(clockModel, clockShader)
        .setTransformation(std::move(clockComposite2))
        .enableClockHands();


    // ground plane

    scene->addDrawable(groundModel, basicShader).setColor(glm::vec3(0.5f, 0.5f, 0.0f));


    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(glm::vec3(4.0f, 4.0f, 0.5f), 0.2f))
        .setColor(glm::vec3(1.0f, 1.0f, 0.0f));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createLoginScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& loginModel = models.get("login");

	ShaderProgram& shader = shaderManager.get("basic");

    auto scene = std::make_unique<Scene>();
    scene->addDrawable(loginModel,shaderManager.get("basic"))
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSolarSystemScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& sunModel = models.get("sun");
    Model& earthModel = models.get("earth");
    Model& moonModel = models.get("moon");
    Model& astronautModel = models.get("astronaut");


	ShaderProgram& shader = shaderManager.get("basic");
	ShaderProgram& lightShader = shaderManager.get("lambert");


    auto scene = std::make_unique<Scene>();

	lightShader.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));

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
    scene->addDrawable(earthModel,lightShader).setTransformation(std::move(earthComposite));

    auto moonComposite = std::make_unique<TransformationComposite>();
    addOrbit(*moonComposite, earthOrbitSpeed, earthOrbitRadius);
    addOrbit(*moonComposite, moonOrbitSpeed, moonOrbitRadius);
    moonComposite->add(std::make_unique<DynamicRotateTransformation>(moonSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(moonScale)));
    scene->addDrawable(moonModel,lightShader).setTransformation(std::move(moonComposite));

    constexpr float astronautOrbitSpeed = 50.0f, astronautOrbitRadius = 3.0f, astronautSpinSpeed = 120.0f, astronautScale = 0.09f;
    auto astronautComposite = std::make_unique<TransformationComposite>();
    addOrbit(*astronautComposite, earthOrbitSpeed, -earthOrbitRadius);
    astronautComposite->add(std::make_unique<DynamicRotateTransformation>(astronautSpinSpeed, glm::vec3(0.0f, -1.0f, 0.0f)));
    astronautComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(astronautScale)));
    astronautComposite->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.0f, -3.1f)));
    astronautComposite->add(std::make_unique<RotateTransformation>(-90.0f,glm::vec3(1.0f, 0.0f, 0.0f)));
    scene->addDrawable(astronautModel, lightShader).setTransformation(std::move(astronautComposite));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createLightScene(ShaderManager& shaderManager, ModelManager& models)
{
    auto scene = std::make_unique<Scene>();
    Model& sphereModel = models.get("sphere");

	ShaderProgram& ambient = shaderManager.get("ambient");
	ShaderProgram& Lambert = shaderManager.get("lambert");
	ShaderProgram& Phong = shaderManager.get("phong");
	ShaderProgram& bPhong = shaderManager.get("blinn");

    ambient.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Lambert.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Phong.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    bPhong.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    scene->addDrawable(sphereModel, ambient)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.5f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    scene->addDrawable(sphereModel, Phong)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, -0.5f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    scene->addDrawable(sphereModel, bPhong)
        .setTransformation(makeStaticTransform(glm::vec3(0.5f, 0.0f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    scene->addDrawable(sphereModel, Lambert)
        .setTransformation(makeStaticTransform(glm::vec3(-0.5f, 0.0f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    return scene;

}
