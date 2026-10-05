#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include <glm/vec3.hpp>
#include <glad/gl.h>

class DrawableObject {
public:
    DrawableObject(Model& model, ShaderProgram& shaderProgram);

    void draw() const;
    
    // Gettery
    float getAngle() const { return angle; }
    glm::vec3 getPosition() const { return position; }
    float getScale() const { return scale; }
    Model& getModel() { return model; }
    ShaderProgram& getShaderProgram() { return shaderProgram; }
    
    // Settery
    void setAngle(float newAngle) { angle = newAngle; }
    void setPosition(glm::vec3 newPosition) { position = newPosition; }
	void setScale(float newScale) { scale = newScale; }
    void setColor(glm::vec3 newColor) { color = newColor; }
    void addAngle(float deltaAngle) { angle += deltaAngle; }
    void addPosition(glm::vec3 deltaPosition) { position += deltaPosition; }

private:
    Model& model;
    ShaderProgram& shaderProgram;
    glm::vec3 position;
    float angle;
    float scale;
    glm::vec3 color;

    mutable GLint angleLoc;
    mutable GLint positionLoc;
	mutable GLint scaleLoc;
    mutable GLint colorLoc;
};