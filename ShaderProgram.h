#pragma once

#include "Shader.h"
#include "IObserver.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <glad/gl.h>

class Camera;

class ShaderProgram : public IObserver {
public:
    ShaderProgram(const char* vertexFile, const char* fragmentFile);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;
    GLuint getProgram() const { return programId; }

	void setCamera(Camera* newCamera);
	void onCameraChanged() override;

    // Přetížené sendUniform funkce
    void sendUniform(GLint location, float value) const;
    void sendUniform(GLint location, int value) const;
    void sendUniform(GLint location, const glm::vec2& vec) const;
    void sendUniform(GLint location, const glm::vec3& vec) const;
    void sendUniform(GLint location, const glm::mat3& mat) const;
    void sendUniform(GLint location, const glm::vec4& vec) const;
	void sendUniform(GLint location, const glm::mat4& mat) const;
	void setLightPosition(const glm::vec3& position);

private:
    Shader vertexShader;
    Shader fragmentShader;
    GLuint programId = 0;

    Camera* camera = nullptr;
    GLint viewMatrixLoc = -1;
	GLint projectionMatrixLoc = -1;
	GLint lightPositionLoc = -1;
	GLint viewPositionLoc = -1;
};