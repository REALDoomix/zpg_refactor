#include "SceneFactory.h"
#include "../Transformations.h"
#include "../Light.h"

std::unique_ptr<Scene> SceneFactory::createTriangleScene(ShaderManager& shaderManager, ModelManager& models) {
    auto scene = std::make_unique<Scene>();
    Model& triangleModel = models.get("triangle");
    ShaderProgram& shader = shaderManager.get("basic");

    auto compositeA = std::make_unique<TransformationComposite>();
    compositeA->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.5f, 0.0f, 0.0f)));
    compositeA->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    scene->addDrawable(triangleModel, shader).setTransformation(std::move(compositeA));

    auto compositeB = std::make_unique<TransformationComposite>();
    compositeB->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    compositeB->add(std::make_unique<TranslateTransformation>(glm::vec3(1.5f, 0.0f, 0.0f)));
    scene->addDrawable(triangleModel, shader).setTransformation(std::move(compositeB));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSphereScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& sphereModel = models.get("sphere");
    ShaderProgram& lightShader = shaderManager.get("light");
    ShaderProgram& basicShader = shaderManager.get("basic");

    auto scene = std::make_unique<Scene>();

    // Centrální koule osvětlovaná polem světel
    scene->addDrawable(sphereModel, lightShader)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.0f, 0.0f), 0.3f))
        .setColor(glm::vec3(0.85f, 0.85f, 0.85f));

    // Světlo 1: Červené statické světlo vlevo
    glm::vec3 redPos(-2.0f, 0.5f, 0.0f);
    glm::vec3 redCol(1.0f, 0.1f, 0.1f);
    scene->addLight(redPos, redCol);
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(redPos, 0.06f))
        .setColor(redCol);

    // Světlo 2: Modré statické světlo vpravo
    glm::vec3 bluePos(2.0f, 0.5f, 0.0f);
    glm::vec3 blueCol(0.1f, 0.3f, 1.0f);
    scene->addLight(bluePos, blueCol);
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(bluePos, 0.06f))
        .setColor(blueCol);

    // Světlo 3: Dynamické žluté světlo obíhající centrální kouli (napojené na transformaci)
    auto orbitTransform = std::make_shared<TransformationComposite>();
    orbitTransform->add(std::make_unique<DynamicRotateTransformation>(60.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    orbitTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.4f, 1.4f)));
    orbitTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.06f)));

    glm::vec3 yellowCol(1.0f, 1.0f, 0.4f);
    scene->addLight(orbitTransform, yellowCol);
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(orbitTransform)
        .setColor(yellowCol);

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
    clockComposite->add(std::make_unique<RotateTransformation>(-90.0f, (glm::vec3(0.0f, 1.0f, 0.0f))));

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

    auto scene = std::make_unique<Scene>();
    scene->addDrawable(loginModel, shaderManager.get("basic"))
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSolarSystemScene(ShaderManager& shaderManager, ModelManager& models) {
    Model& sunModel = models.get("sun");
    Model& earthModel = models.get("earth");
    Model& moonModel = models.get("moon");
    Model& astronautModel = models.get("astronaut");

    ShaderProgram& shader = shaderManager.get("basic");
    ShaderProgram& lightShader = shaderManager.get("phong");

    auto scene = std::make_unique<Scene>();

    lightShader.setLightPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    constexpr float earthOrbitSpeed = 20.0f, earthOrbitRadius = 4.0f, earthSpinSpeed = 90.0f, earthScale = 0.3f;
    constexpr float moonOrbitSpeed = 80.0f, moonOrbitRadius = 0.8f, moonSpinSpeed = 40.0f, moonScale = 0.1f;

    auto sunComposite = std::make_unique<TransformationComposite>();
    sunComposite->add(std::make_unique<DynamicRotateTransformation>(10.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    sunComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(1.0f)));
    scene->addDrawable(sunModel, shader).setTransformation(std::move(sunComposite));

    auto earthComposite = std::make_unique<TransformationComposite>();
    addOrbit(*earthComposite, earthOrbitSpeed, earthOrbitRadius);
    earthComposite->add(std::make_unique<DynamicRotateTransformation>(earthSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    earthComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(earthScale)));
    scene->addDrawable(earthModel, lightShader).setTransformation(std::move(earthComposite));

    auto moonComposite = std::make_unique<TransformationComposite>();
    addOrbit(*moonComposite, earthOrbitSpeed, earthOrbitRadius);
    addOrbit(*moonComposite, moonOrbitSpeed, moonOrbitRadius);
    moonComposite->add(std::make_unique<DynamicRotateTransformation>(moonSpinSpeed, glm::vec3(0.0f, 1.0f, 0.0f)));
    moonComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(moonScale)));
    scene->addDrawable(moonModel, lightShader).setTransformation(std::move(moonComposite));

    constexpr float astronautOrbitSpeed = 50.0f, astronautOrbitRadius = 3.0f, astronautSpinSpeed = 120.0f, astronautScale = 0.09f;
    auto astronautComposite = std::make_unique<TransformationComposite>();
    addOrbit(*astronautComposite, earthOrbitSpeed, -earthOrbitRadius);
    astronautComposite->add(std::make_unique<DynamicRotateTransformation>(astronautSpinSpeed, glm::vec3(0.0f, -1.0f, 0.0f)));
    astronautComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(astronautScale)));
    astronautComposite->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.0f, -3.1f)));
    astronautComposite->add(std::make_unique<RotateTransformation>(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
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
    // spodní koule
    scene->addDrawable(sphereModel, Phong)
        .setTransformation(makeStaticTransform(glm::vec3(0.0f, -0.5f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    // pravá koule
    scene->addDrawable(sphereModel, Lambert)
        .setTransformation(makeStaticTransform(glm::vec3(0.5f, 0.0f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    // levá koule
    scene->addDrawable(sphereModel, bPhong)
        .setTransformation(makeStaticTransform(glm::vec3(-0.5f, 0.0f, 0.0f), 0.1f)).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    return scene;
}

std::unique_ptr<Scene> SceneFactory::createNormalDemoScene(ShaderManager& shaderManager, ModelManager& models)
{
    auto scene = std::make_unique<Scene>();
    Model& sphereModel = models.get("sphere");

    ShaderProgram& badNormalShader = shaderManager.get("bad_normal");
    ShaderProgram& correctNormalShader = shaderManager.get("phong");
    ShaderProgram& basicShader = shaderManager.get("basic");

    // Jedno světlo uprostřed
    glm::vec3 lightPos(0.0f, 1.0f, 1.5f);
    scene->addLight(lightPos, glm::vec3(1.0f, 1.0f, 1.0f));
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(lightPos, 0.05f))
        .setColor(glm::vec3(1.0f, 1.0f, 0.8f));

    // Vlevo: CHYBNÝ převod normál (neuniformní škálování: zploštělá koule mat3(modelMatrix) * vn)
    auto badComposite = std::make_unique<TransformationComposite>();
    badComposite->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.0f, 0.0f, 0.0f)));
    badComposite->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 1.0f, 1.0f)));
    badComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(0.35f, 0.08f, 0.35f)));
    scene->addDrawable(sphereModel, badNormalShader)
        .setTransformation(std::move(badComposite))
        .setColor(glm::vec3(0.9f, 0.2f, 0.2f)); // Červený objekt s chybným převodem

    // Vpravo: SPRÁVNÝ převod normál (identické neuniformní škálování a rotace, ale s normalMatrix)
    auto correctComposite = std::make_unique<TransformationComposite>();
    correctComposite->add(std::make_unique<TranslateTransformation>(glm::vec3(1.0f, 0.0f, 0.0f)));
    correctComposite->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 1.0f, 1.0f)));
    correctComposite->add(std::make_unique<ScaleTransformation>(glm::vec3(0.35f, 0.08f, 0.35f)));
    scene->addDrawable(sphereModel, correctNormalShader)
        .setTransformation(std::move(correctComposite))
        .setColor(glm::vec3(0.2f, 0.8f, 0.2f)); // Zelený objekt se správným převodem

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSpecularBleedDemoScene(ShaderManager& shaderManager, ModelManager& models)
{
    auto scene = std::make_unique<Scene>();
    Model& sphereModel = models.get("sphere");

    ShaderProgram& bleedShader = shaderManager.get("phong_bleed");
    ShaderProgram& clippedShader = shaderManager.get("phong_clipped");
    ShaderProgram& basicShader = shaderManager.get("basic");

    // Světlo umístěné ZA koulemi (v záporné ose Z, např. z = -2.0f).
    // Z pohledu kamery (z = +4.0f) se díváme na odvrácenou, neosvětlenou stranu koulí.
    glm::vec3 lightPos(0.0f, 0.2f, -2.0f);
    scene->addLight(lightPos, glm::vec3(1.0f, 1.0f, 1.0f));

    // Zobrazení pozice světla za koulemi (žlutá zářící kulička)
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(makeStaticTransform(lightPos, 0.08f))
        .setColor(glm::vec3(1.0f, 1.0f, 0.2f));

    // Vlevo: CHYBNÉ (a) - Odvrácená strana kuličky s mocninou na 1 a BEZ ořezání zrcadlové složky.
    // Zrcadlový odlesk nepřirozeně prosvítá na neosvětlenou stranu!
    scene->addDrawable(sphereModel, bleedShader)
        .setTransformation(makeStaticTransform(glm::vec3(-1.0f, 0.0f, 0.0f), 0.35f))
        .setColor(glm::vec3(0.7f, 0.15f, 0.15f)); // Červená koule s přesvitem

    // Vpravo: OPRAVENÉ (b) - Ořezání přesvitu na odvrácené straně (dot(N, L) > 0.0).
    // Odvrácená strana zůstává tmavá bez parazitního odlesku!
    scene->addDrawable(sphereModel, clippedShader)
        .setTransformation(makeStaticTransform(glm::vec3(1.0f, 0.0f, 0.0f), 0.35f))
        .setColor(glm::vec3(0.15f, 0.65f, 0.25f)); // Zelená koule s ořezáním

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createNightForestScene(ShaderManager& shaderManager, ModelManager& models)
{
    auto scene = std::make_unique<Scene>();
    Model& treeModel = models.get("tree");
    Model& bushModel = models.get("bush");
    Model& sphereModel = models.get("sphere");
    Model& groundModel = models.get("ground");

    ShaderProgram& nightTreeShader = shaderManager.get("night_tree");
    ShaderProgram& nightForestShader = shaderManager.get("night_forest");
    ShaderProgram& basicShader = shaderManager.get("basic");

	auto fireflyTransform = std::make_shared<TransformationComposite>();
	constexpr float fireFlyOrbitSpeed = 35.0f, fireflyOrbitRadius = 3.0f;
	addOrbit(*fireflyTransform, fireFlyOrbitSpeed, fireflyOrbitRadius);

    fireflyTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.04f)));

    // Světlo světlušky: teplé žlutozelené světlo s výrazným útlumem pro omezený dosvit
    auto fireflyLight = scene->addLight(fireflyTransform, glm::vec3(1.0f, 0.95f, 0.35f));
    fireflyLight->constant = 0.1f;
    fireflyLight->linear = 0.18f;
    fireflyLight->quadratic = 0.125f;

    // Vizuální tělo světlušky (zářící kulička)
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(fireflyTransform)
        .setColor(glm::vec3(1.0f, 1.0f, 0.5f));


    auto fireflyTransform2 = std::make_shared<TransformationComposite>();

    fireflyTransform2->add(std::make_unique<RotateTransformation>(180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));

    addOrbit(*fireflyTransform2, fireFlyOrbitSpeed, fireflyOrbitRadius);

    fireflyTransform2->add(std::make_unique<ScaleTransformation>(glm::vec3(0.04f)));

    // Světlo světlušky: teplé žlutozelené světlo s výrazným útlumem pro omezený dosvit
    auto fireflyLight2 = scene->addLight(fireflyTransform2, glm::vec3(1.0f, 0.95f, 0.35f));
    fireflyLight2->constant = 0.1f;
    fireflyLight2->linear = 0.18f;
    fireflyLight2->quadratic = 0.125f;

    // Vizuální tělo světlušky (zářící kulička)
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(fireflyTransform2)
        .setColor(glm::vec3(1.0f, 1.0f, 0.5f));

    constexpr int treeCount = 55;
    for (int i = 0; i < treeCount; i++) {
        scene->addDrawable(treeModel, nightTreeShader)
            .setTransformation(makeRandomTransform(-7.0f, 7.0f, -1.0f, -5.0f, 2.0f, 0.08f, 0.14f));
    }

    constexpr int bushCount = 40;
    for (int i = 0; i < bushCount; i++) {
        scene->addDrawable(bushModel, nightForestShader)
            .setTransformation(makeRandomTransform(-5.0f, 5.0f, -1.0f, -4.0f, 1.0f, 0.4f, 0.6f))
            .setColor(glm::vec3(0.05f, 0.35f, 0.05f));
    }

    // Země pod lesem
    auto groundTransform = std::make_unique<TransformationComposite>();
    groundTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, -1.0f, 0.0f)));
    groundTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(15.0f, 1.0f, 15.0f)));
    scene->addDrawable(groundModel, nightForestShader)
        .setTransformation(std::move(groundTransform))
        .setColor(glm::vec3(0.08f, 0.18f, 0.08f));

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createTextureDemoScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures) {
    auto scene = std::make_unique<Scene>();
    Model& plainModel = models.get("plain");
    ShaderProgram& texShader = shaderManager.get("texture");
    auto woodenTex = textures.get("wooden_fence");

    // Otáčející se deska s UV mapováním a texturou dřevěného plotu
    auto spinTransform = std::make_shared<TransformationComposite>();
    spinTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 1.0f, 0.0f)));
    spinTransform->add(std::make_unique<DynamicRotateTransformation>(45.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    spinTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(1.5f)));
    scene->addDrawable(plainModel, texShader)
        .setTransformation(spinTransform)
        .setTexture(woodenTex, 0, "textureUnitID");

    // Vodorovná podlaha s texturou
    auto floorTransform = std::make_shared<TransformationComposite>();
    floorTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, -0.5f, 0.0f)));
    floorTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(4.0f)));
    scene->addDrawable(plainModel, texShader)
        .setTransformation(floorTransform)
        .setTexture(woodenTex, 0, "textureUnitID");

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createMultiTextureScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures) {
    auto scene = std::make_unique<Scene>();
    Model& plainModel = models.get("plain");
    ShaderProgram& multiShader = shaderManager.get("multi_texture");

    auto tex0 = textures.get("wooden_fence");
    auto tex1 = textures.get("shrek");

    auto transform = std::make_shared<TransformationComposite>();
    transform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.5f, 0.0f)));
    transform->add(std::make_unique<DynamicRotateTransformation>(30.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    transform->add(std::make_unique<ScaleTransformation>(glm::vec3(2.0f)));

    scene->addDrawable(plainModel, multiShader)
        .setTransformation(transform)
        .addTexture(tex0, 0, "textureUnit0")
        .addTexture(tex1, 1, "textureUnit1");

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createShrekScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures) {
    auto scene = std::make_unique<Scene>();
    ShaderProgram& texPhong = shaderManager.get("texture_phong");
    ShaderProgram& basicShader = shaderManager.get("basic");

    Model& shrekModel = models.get("shrek");
    Model& toiletModel = models.get("toilet");
    Model& plainModel = models.get("plain");
    Model& sphereModel = models.get("sphere");

    auto shrekTex = textures.get("shrek");
    auto toiletTex = textures.get("toilet");
    auto fenceTex = textures.get("wooden_fence");

    // Světlo na scéně
    auto lightTransform = std::make_shared<TransformationComposite>();
    lightTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 4.0f, 2.0f)));
    auto light = scene->addLight(lightTransform, glm::vec3(1.0f, 0.95f, 0.85f));
    light->constant = 1.0f;
    light->linear = 0.09f;
    light->quadratic = 0.032f;

    // Vizuální kulička pro světlo
    auto sphereTransform = std::make_shared<TransformationComposite>();
    sphereTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 4.0f, 2.0f)));
    sphereTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.15f)));
    scene->addDrawable(sphereModel, basicShader)
        .setTransformation(sphereTransform)
        .setColor(glm::vec3(1.0f, 1.0f, 0.8f));

    // Shrek
    auto shrekTransform = std::make_shared<TransformationComposite>();
    shrekTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.0f, 0.0f, 0.0f)));
    shrekTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.8f)));
    scene->addDrawable(shrekModel, texPhong)
        .setTransformation(shrekTransform)
        .setTexture(shrekTex, 0, "textureUnitID");

    // Toaleta
    auto toiletTransform = std::make_shared<TransformationComposite>();
    toiletTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(1.2f, 0.0f, 0.0f)));
    toiletTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.8f)));
    scene->addDrawable(toiletModel, texPhong)
        .setTransformation(toiletTransform)
        .setTexture(toiletTex, 0, "textureUnitID");

    // Fiona (pokud je dostupná)
    if (models.has("fiona") && textures.has("fiona")) {
        Model& fionaModel = models.get("fiona");
        auto fionaTex = textures.get("fiona");
        auto fionaTransform = std::make_shared<TransformationComposite>();
        fionaTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(-2.8f, 0.0f, 0.0f)));
        fionaTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.8f)));
        scene->addDrawable(fionaModel, texPhong)
            .setTransformation(fionaTransform)
            .setTexture(fionaTex, 0, "textureUnitID");
    }

    // Podlaha
    auto floorTransform = std::make_shared<TransformationComposite>();
    floorTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.0f, 0.0f)));
    floorTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(6.0f)));
    scene->addDrawable(plainModel, texPhong)
        .setTransformation(floorTransform)
        .setTexture(fenceTex, 0, "textureUnitID");

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createSkyboxScene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures) {
    auto scene = std::make_unique<Scene>();
    ShaderProgram& skyboxShader = shaderManager.get("skybox");
    ShaderProgram& texPhong = shaderManager.get("texture_phong");

    Model& skyboxModel = models.get("skybox");
    Model& shrekModel = models.get("shrek");
    Model& toiletModel = models.get("toilet");
    Model& plainModel = models.get("plain");

    auto skyboxTex = textures.get("skybox");
    auto shrekTex = textures.get("shrek");
    auto toiletTex = textures.get("toilet");
    auto fenceTex = textures.get("wooden_fence");

    // Skybox krychle obklopující scénu
    auto skyboxTransform = std::make_shared<TransformationComposite>();
    skyboxTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(50.0f)));
    scene->addDrawable(skyboxModel, skyboxShader)
        .setTransformation(skyboxTransform)
        .setTexture(skyboxTex, 0, "skybox")
        .setSkyboxMode(true);

    // Světlo pro modely uvnitř skyboxu
    auto lightTransform = std::make_shared<TransformationComposite>();
    lightTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 5.0f, 3.0f)));
    scene->addLight(lightTransform, glm::vec3(1.0f, 1.0f, 1.0f));

    // Shrek
    auto shrekTransform = std::make_shared<TransformationComposite>();
    shrekTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.0f, 0.0f, 0.0f)));
    shrekTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.8f)));
    scene->addDrawable(shrekModel, texPhong)
        .setTransformation(shrekTransform)
        .setTexture(shrekTex, 0, "textureUnitID");

    // Toaleta
    auto toiletTransform = std::make_shared<TransformationComposite>();
    toiletTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(1.2f, 0.0f, 0.0f)));
    toiletTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.8f)));
    scene->addDrawable(toiletModel, texPhong)
        .setTransformation(toiletTransform)
        .setTexture(toiletTex, 0, "textureUnitID");

    // Podlaha
    auto floorTransform = std::make_shared<TransformationComposite>();
    floorTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.0f, 0.0f)));
    floorTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(5.0f)));
    scene->addDrawable(plainModel, texPhong)
        .setTransformation(floorTransform)
        .setTexture(fenceTex, 0, "textureUnitID");

    return scene;
}

std::unique_ptr<Scene> SceneFactory::createEB408Scene(ShaderManager& shaderManager, ModelManager& models, TextureManager& textures) {
    auto scene = std::make_unique<Scene>();
    ShaderProgram& texPhong = shaderManager.get("classroom"); // use classroom shader for EB408
    ShaderProgram& classroom = shaderManager.get("classroom");
    ShaderProgram& clockShader = shaderManager.get("clock");
    ShaderProgram& basicShader = shaderManager.get("basic");
    ShaderProgram& skyboxShader = shaderManager.get("skybox");

    Model& skyboxModel = models.get("skybox");
    Model& roomModel = models.get("EB408");
    Model& doorModel = models.get("Door");
    Model& whiteboardModel = models.get("Whiteboard");
    Model& tableModel = models.get("table");
    Model& chairModel = models.get("OrangeChair");
    Model& clockModel = models.get("clock");
    Model& shrekModel = models.get("shrek");
    Model& sphereModel = models.get("sphere");

    auto skyboxTex = textures.get("skybox");
    auto roomTex = textures.get("EB408");
    auto doorTex = textures.get("Door");
    auto whiteboardTex = textures.get("Whiteboard");
    auto tableTex = textures.get("table");
    auto chairTex = textures.get("OrangeChair");
    auto shrekTex = textures.get("shrek");

    auto skyboxTransform = std::make_shared<TransformationComposite>();
    skyboxTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(50.0f)));
    scene->addDrawable(skyboxModel, skyboxShader)
        .setTransformation(skyboxTransform)
        .setTexture(skyboxTex, 0, "skybox")
        .setSkyboxMode(true);

    // 1. Model ucebny EB408 (steny, podlaha, strop, okna)
    auto roomTransform = std::make_shared<TransformationComposite>();
    roomTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(3.0f, 0.0f, 4.05f)));
    roomTransform->add(std::make_unique<RotateTransformation>(-90.0f, glm::vec3(0.0f,1.0f,0.0f)));
    scene->addDrawable(roomModel, classroom)
        .setTransformation(roomTransform)
        .setTexture(roomTex, 0, "textureUnitID");

    // 2. Dvere v ramu na zadni stene
    auto doorTransform = std::make_shared<TransformationComposite>();
    doorTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(-4.8f, 0.0f, 1.6f)));
    doorTransform->add(std::make_unique<RotateTransformation>(-90.0f, glm::vec3(0.0f,1.0f,0.0f)));
    scene->addDrawable(doorModel, texPhong)
        .setTransformation(doorTransform)
        .setTexture(doorTex, 0, "textureUnitID");

    // 3. Tabule s tematem ZPG (Phonguv osvetlovaci model)
    auto whiteboardTransform = std::make_shared<TransformationComposite>();
    whiteboardTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 0.0f, 0.25f)));
    whiteboardTransform->add(std::make_unique<RotateTransformation>(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    scene->addDrawable(whiteboardModel, texPhong)
        .setTransformation(whiteboardTransform)
        .setTexture(whiteboardTex, 0, "textureUnitID");

    // 4. Hodiny nad tabuli s animovanymi rucickami
    auto clockTransform = std::make_shared<TransformationComposite>();
    clockTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(0.0f, 2.75f, 0.05f)));
    clockTransform->add(std::make_unique<RotateTransformation>(180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    clockTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.18f)));
    scene->addDrawable(clockModel, clockShader)
        .setTransformation(clockTransform)
        .enableClockHands();

    // 5. Prednasejici Shrek u tabule
    auto shrekTransform = std::make_shared<TransformationComposite>();
    shrekTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(-1.8f, 0.0f, 0.9f)));
    shrekTransform->add(std::make_unique<RotateTransformation>(25.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    shrekTransform->add(std::make_unique<ScaleTransformation>(glm::vec3(0.7f)));
    scene->addDrawable(shrekModel, texPhong)
        .setTransformation(shrekTransform)
        .setTexture(shrekTex, 0, "textureUnitID");

    // 6. Ucitelsky stul a zidle
    auto teacherDeskTransform = std::make_shared<TransformationComposite>();
    teacherDeskTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(1.8f, 0.0f, 1.5f)));
    scene->addDrawable(tableModel, texPhong)
        .setTransformation(teacherDeskTransform)
        .setTexture(tableTex, 0, "textureUnitID");

    auto teacherChairTransform = std::make_shared<TransformationComposite>();
    teacherChairTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(1.8f, 0.0f, 0.75f)));
    scene->addDrawable(chairModel, texPhong)
        .setTransformation(teacherChairTransform)
        .setTexture(chairTex, 0, "textureUnitID");


    // 7. Studentske lavice a oranzove zidle (2 rady x 3 sloupce = 6 lavic, 12 zidli)
    float colX[2] = { -1.8f, 1.8f };
    float rowZ[3] = { 2.8f, 4.3f, 5.8f };

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 2; ++c) {
            float x = colX[c];
            float z = rowZ[r];

            // Stul
            auto deskTransform = std::make_shared<TransformationComposite>();
            deskTransform->add(std::make_unique<TranslateTransformation>(glm::vec3(x, 0.0f, z)));
            scene->addDrawable(tableModel, texPhong)
                .setTransformation(deskTransform)
                .setTexture(tableTex, 0, "textureUnitID");

            // Leva zidle (otocena o 180 stupnu celem ke stolu a k tabuli)
            auto chairLeft = std::make_shared<TransformationComposite>();
            chairLeft->add(std::make_unique<TranslateTransformation>(glm::vec3(x - 0.45f, 0.0f, z + 0.55f)));
            chairLeft->add(std::make_unique<RotateTransformation>(180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
            scene->addDrawable(chairModel, texPhong)
                .setTransformation(chairLeft)
                .setTexture(chairTex, 0, "textureUnitID");

            // Prava zidle
            auto chairRight = std::make_shared<TransformationComposite>();
            chairRight->add(std::make_unique<TranslateTransformation>(glm::vec3(x + 0.45f, 0.0f, z + 0.55f)));
            chairRight->add(std::make_unique<RotateTransformation>(180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
            scene->addDrawable(chairModel, texPhong)
                .setTransformation(chairRight)
                .setTexture(chairTex, 0, "textureUnitID");
        }
    }

    // 8. Osvetleni ucebny (4 stropni zarivkova svetla)
    glm::vec3 lightPositions[4] = {
        glm::vec3(-1.8f, 3.1f, 2.2f),
        glm::vec3( 1.8f, 3.1f, 2.2f),
        glm::vec3(-1.8f, 3.1f, 5.2f),
        glm::vec3( 1.8f, 3.1f, 5.2f)
    };
    for (int i = 0; i < 4; ++i) {
        auto light = scene->addLight(lightPositions[i], glm::vec3(0.95f, 0.95f, 0.92f));
        light->constant = 1.0f;
        light->linear = 0.07f;
        light->quadratic = 0.02f;
    }

    return scene;
}
