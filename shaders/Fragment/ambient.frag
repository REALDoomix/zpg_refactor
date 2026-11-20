#version 330 core

in vec3 fragColor;
uniform vec3 objectColor;
out vec4 out_Color;

void main(void){
    vec3 baseColor = (objectColor == vec3(0.0)) ? fragColor : objectColor;
    vec3 ambient = 0.1 * baseColor;
    out_Color = vec4(ambient, 1.0);
}