#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

uniform vec3 lightPosition;
uniform vec3 objectColor;

out vec4 out_Color;

void main(void){
    vec3 baseColor = (objectColor == vec3(0.0)) ? fragColor : objectColor;

    vec3 lightDir = normalize(lightPosition - vec3(worldPosition));
    float diff = max(dot(normalize(worldNormal), lightDir), 0.0);

    vec3 diffuse = diff * baseColor;
    vec3 ambient = 0.1 * baseColor;

    out_Color = vec4(ambient + diffuse, 1.0);
}