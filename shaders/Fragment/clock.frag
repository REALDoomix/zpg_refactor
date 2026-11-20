#version 330 core

in vec3 vPartId;
out vec4 outColor;

void main()
{
    ivec3 id = ivec3(round(vPartId));
    vec3 color;

    if (id == ivec3(1, 1, 1))        color = vec3(1.0);                    // face
    else if (id == ivec3(0, 0, 0))   color = vec3(0.58);                   // rim, numerals, ticks
    else if (id == ivec3(1, 1, 0))   color = vec3(0.53, 0.84, 0.79);       // "KI" logo + bars (teal)
    else if (id == ivec3(1, 0, 0))   color = vec3(1.0, 0.0, 0.0);          // second hand
    else                             color = vec3(0.0);                    // hour + minute hands (black)

    outColor = vec4(color, 1.0);
}
