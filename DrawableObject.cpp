#include "DrawableObject.h"
#include <iostream>


DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram)
    : model(model), shaderProgram(shaderProgram), position(0.0f, 0.0f, 0.0f), angle(0.0f), scale(1.0f), color(0.0f, 0.0f, 0.0f) {
    // Cachuj lokace jednou při vytvoření
    angleLoc = glGetUniformLocation(shaderProgram.getProgram(), "angle");
    positionLoc = glGetUniformLocation(shaderProgram.getProgram(), "position_offset");
	scaleLoc = glGetUniformLocation(shaderProgram.getProgram(), "scale");
	colorLoc = glGetUniformLocation(shaderProgram.getProgram(), "objectColor");

    std::cout << "colorLoc: " << colorLoc << " | color: ("
        << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
}

void DrawableObject::draw() const {
    shaderProgram.use();
    
    // Jen pošli uniformy bez volání glGetUniformLocation
    shaderProgram.sendUniform(angleLoc, angle);
    shaderProgram.sendUniform(positionLoc, position);
	shaderProgram.sendUniform(scaleLoc, scale);
    shaderProgram.sendUniform(colorLoc, color);
    
    model.draw();
}