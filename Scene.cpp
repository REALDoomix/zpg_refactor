#include "Scene.h"

#include "Models/gift.h"
#include "Models/tree.h"

Scene::Scene()
    : shaderProgram("shaders/basic.vert", "shaders/basic.frag"),
      giftModel(::gift, sizeof(::gift)),
      treeModel(::tree, sizeof(::tree)),
      giftTransformation(),
      treeTransformation(),
      gift(giftModel, shaderProgram, giftTransformation),
      tree(treeModel, shaderProgram, treeTransformation) {
    
    // Nastav počáteční pozice
    treeTransformation.setPosition(glm::vec2(0.5f, -0.5f));
}

void Scene::draw() const {
    gift.draw();
    tree.draw();
}