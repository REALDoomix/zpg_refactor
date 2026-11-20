#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D textureUnit0;
uniform sampler2D textureUnit1;
uniform float blendFactor;

out vec4 FragColor;

void main()
{
    vec4 col0 = texture(textureUnit0, uv);
    vec4 col1 = texture(textureUnit1, uv);
    float factor = (blendFactor >= 0.0 && blendFactor <= 1.0) ? blendFactor : 0.5;
    FragColor = mix(col0, col1, factor);
}
