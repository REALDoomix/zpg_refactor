//Vertex shader
layout(location = 0) in vec3 vp; //vertex position
layout(location = 1) in vec3 vcol; // vertex color
layout(location = 2) in vec3 vn; //vertex normal


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec3 fragColor;

void main(void){
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp,1.0f);
    worldPosition = modelMatrix * vec4(vp,1.0f);
    worldNormal = vp;
    fragColor = vcol;
}