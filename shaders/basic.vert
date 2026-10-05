#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform vec3 position_offset;  // Posun objektu
uniform float angle;            // Úhel rotace v stupních
uniform float scale;            // Měřítko objektu

out vec3 vertexColor;

void main()
{
    // Převeď stupně na radiány
    float radians = radians(angle);
    float cosA = cos(radians);
    float sinA = sin(radians);
    
    //float x_rotated = position.x * cosA - position.y * sinA;
    //float y_rotated = position.x * sinA + position.y * cosA;


    float x_rotated = cosA * position.x + sinA * position.z;
    float y_rotated = position.y;
    float z_rotated = -sinA * position.x + cosA * position.z;

    
    // Aplikuj posun
    vec3 transformed = vec3(x_rotated + position_offset.x, 
                            y_rotated + position_offset.y, 
                            z_rotated + position_offset.z);
    
    vertexColor = color;
    gl_Position = vec4(transformed * scale, 1.0);
}