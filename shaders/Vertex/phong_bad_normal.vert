#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vcol;
layout(location = 2) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec3 fragColor;

void main(void){
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    worldPosition = modelMatrix * vec4(vp, 1.0f);
    // CHYBNÝ PŘEVOD NORMÁL:
    // Obyčejné násobení mat3(modelMatrix) bez transponované inverze (M^-1)^T.
    // Při neuniformním škálování (např. zploštělá nebo natažená koule) normály NEMAJÍ správný směr a nejsou kolmé k povrchu!
    worldNormal = normalize(mat3(modelMatrix) * vn);
    fragColor = vcol;
}
