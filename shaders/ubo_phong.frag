#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

uniform vec3 viewPosition;
uniform vec3 objectColor;
uniform int numberOfLights;

#define MAX_LIGHTS 4
struct Light
{
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};

layout (std140) uniform LightBuffer
{
    Light lights[MAX_LIGHTS];
};

out vec4 out_Color;

void main()
{
    vec3 baseColor = (objectColor == vec3(0.0)) ? fragColor : objectColor;
    vec3 normal = normalize(worldNormal);
    vec3 viewDir = normalize(viewPosition - vec3(worldPosition));

    vec3 ambient = 0.1 * baseColor;
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    int count = clamp(numberOfLights, 0, MAX_LIGHTS);
    for (int i = 0; i < count; ++i)
    {
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightDir = normalize(lightPos - vec3(worldPosition));

        float diff = max(dot(normal, lightDir), 0.0);
        totalDiffuse += diff * lights[i].diffuse.rgb * baseColor;

        if (diff > 0.0)
        {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            totalSpecular += spec * lights[i].specular.rgb;
        }
    }

    out_Color = vec4(ambient + totalDiffuse + totalSpecular, 1.0);
}
