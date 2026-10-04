#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
public:
    DrawableObject(Model& model, ShaderProgram& shaderProgram, Transformation& transformation)
        : model(model), shaderProgram(shaderProgram), transformation(transformation) {
    }

    void draw() const;
    Transformation& getTransformation() { return transformation; }

private:
    Model& model;
    ShaderProgram& shaderProgram;
    Transformation& transformation;
};