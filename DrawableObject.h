// DrawableObject.h
#pragma once
#include "Transformations.h"
#include <memory>
#include <vector>
#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

struct TextureBinding {
    std::shared_ptr<Texture> texture;
    GLuint unit = 0;
    std::string uniformName = "textureUnitID";
};

class DrawableObject {
public:
    DrawableObject(Model& model, ShaderProgram& shaderProgram);

    void update(float deltaTime);
    void draw() const;

    DrawableObject& setTransformation(std::shared_ptr<ITransformation> transform);
    std::shared_ptr<ITransformation> getTransformation() const { return transformation; }
    ShaderProgram& getShaderProgram() const { return shaderProgram; }

    DrawableObject& setColor(const glm::vec3& c);
    DrawableObject& enableClockHands();

    DrawableObject& setTexture(std::shared_ptr<Texture> tex, GLuint unit = 0, const std::string& uniformName = "textureUnitID");
    DrawableObject& addTexture(std::shared_ptr<Texture> tex, GLuint unit = 0, const std::string& uniformName = "textureUnitID");
    DrawableObject& clearTextures();
    DrawableObject& setSkyboxMode(bool enabled);

private:
    Model& model;
    ShaderProgram& shaderProgram;
    std::shared_ptr<ITransformation> transformation;
    glm::vec3 color;

    std::vector<TextureBinding> textureBindings;
    bool isSkyboxMode = false;

    bool isClock = false;
    GLint hourAngleLoc = -1, minuteAngleLoc = -1, secondAngleLoc = -1;
    float hourAngle = 0.0f, minuteAngle = 0.0f, secondAngle = 0.0f;

    GLint modelMatrixLoc;
    GLint colorLoc;
    GLint normalMatrixLoc;
};
