#version 330 core

in vec3 vertexColor;

uniform vec3 objectColor;

out vec4 FragColor;

void main()
{

    if(objectColor.r == 0.0 && objectColor.g == 0.0 && objectColor.b == 0.0){
        FragColor = vec4(abs(vertexColor), 1.0);
    }
    else {
        FragColor = vec4(objectColor, 1.0);
    }
}