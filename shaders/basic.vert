#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform vec2 position_offset;  // Posun objektu
uniform float angle;            // Úhel rotace v stupních

out vec3 vertexColor;

void main()
{
    // Převeď stupně na radiány
    float radians = radians(angle);
    float cosA = cos(radians);
    float sinA = sin(radians);
    
    float x_rotated = position.x * cosA - position.y * sinA;
    float y_rotated = position.x * sinA + position.y * cosA;
    
    // Aplikuj posun
    vec3 transformed = vec3(x_rotated + position_offset.x, 
                            y_rotated + position_offset.y, 
                            position.z);
    
    vertexColor = color;
    gl_Position = vec4(transformed, 1.0);
}