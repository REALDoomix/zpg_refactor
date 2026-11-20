#version 330 core



layout(location = 0) in vec3 inPosition;

layout(location = 1) in vec3 inNormal;

layout(location = 2) in vec3 inColor;



uniform mat4 modelMatrix;

uniform mat4 viewMatrix;

uniform mat4 projectionMatrix;



uniform float uHourAngle;

uniform float uMinuteAngle;

uniform float uSecondAngle;



out vec3 vPartId;



mat2 rot(float a)

{

float c = cos(a), s = sin(a);

return mat2(c, s, -s, c);

}







void main()

{

vec3 pos = inPosition;

ivec3 id = ivec3(round(inColor));



pos.x = -pos.x;



if (id == ivec3(0, 0, 1)) pos.xy = rot(uHourAngle) * pos.xy;

else if (id == ivec3(0, 1, 0)) pos.xy = rot(uMinuteAngle) * pos.xy;

else if (id == ivec3(1, 0, 0)) pos.xy = rot(uSecondAngle) * pos.xy;



vPartId = inColor;

gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);

}