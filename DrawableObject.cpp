#include "DrawableObject.h"
#include <glm/vec2.hpp>

void DrawableObject::draw() const {
    shaderProgram.use();
    
    // Pošli úhel a pozici do shaderu
    GLint angleLoc = glGetUniformLocation(shaderProgram.getProgram(), "angle");
    GLint posLoc = glGetUniformLocation(shaderProgram.getProgram(), "position_offset");
    
    glm::vec2 pos = transformation.getPosition();
    glUniform1f(angleLoc, transformation.getAngle());
    glUniform2f(posLoc, pos.x, pos.y);
    
    model.draw();
}