#pragma once

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class Scene {
public:
    Scene();

    void draw() const;
    DrawableObject& getGift() { return gift; }
    DrawableObject& getTree() { return tree; }

private:
    ShaderProgram shaderProgram;
	// pridat vector modelu a vector transformationu, aby se dalo pridavat vice objektu
    Model giftModel;
    Model treeModel;
    Transformation giftTransformation;
    Transformation treeTransformation;
    DrawableObject gift;
    DrawableObject tree;
};