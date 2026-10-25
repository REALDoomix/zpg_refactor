#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vcol;
layout(location = 2) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec3 fragColor;

void main(void){
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    worldPosition = modelMatrix * vec4(vp, 1.0f);
    worldNormal = normalize(normalMatrix * vn);  // fixed: vn, not vp; plus proper world-space transform
    fragColor = vcol;
}