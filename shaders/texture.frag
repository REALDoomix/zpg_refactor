#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D textureUnitID;

out vec4 FragColor;

void main()
{
    FragColor = texture(textureUnitID, uv);
}
