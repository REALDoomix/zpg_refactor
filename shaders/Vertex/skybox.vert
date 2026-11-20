#version 330 core
layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
    texCoords = position;
    mat4 viewNoTrans = mat4(mat3(viewMatrix));
    vec4 pos = projectionMatrix * viewNoTrans * modelMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
