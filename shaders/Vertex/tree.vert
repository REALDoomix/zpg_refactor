#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int trunkVertexCount = 0;

out vec3 vertexColor;

void main()
{
    if (gl_VertexID > 36443)
        vertexColor = vec3(0.f, 0.5f, 0.f);  // leaves — green
    else
        vertexColor = vec3(0.7f, 0.3f, 0.f);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}