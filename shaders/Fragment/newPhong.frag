#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

struct Light {
    int type;           // 0 = point, 1 = directional, 2 = spotlight
    vec3 position;
    vec3 direction;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float spotCutoff;
};

#define MAX_LIGHTS 8
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPosition;
uniform vec3 objectColor;

out vec4 out_Color;

void main(void){
    vec3 baseColor = (objectColor == vec3(0.0)) ? fragColor : objectColor;
    vec3 normal = normalize(worldNormal);
    vec3 viewDir = normalize(viewPosition - vec3(worldPosition));

    // Globální ambientní složka
    vec3 totalLight = 0.1 * baseColor;

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir;
        float attenuation = 1.0;

        if (lights[i].type == 1) { // Directional light
            lightDir = normalize(-lights[i].direction);
        } else { // Point light (0) or Spotlight (2)
            vec3 lightToPos = lights[i].position - vec3(worldPosition);
            float distance = length(lightToPos);
            lightDir = normalize(lightToPos);
            attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
        }

        // Difúzní složka
        float diff = max(dot(normal, lightDir), 0.0);

        // Spekulární složka s ořezáním na odvrácené straně
        vec3 specular = vec3(0.0);
        if (diff > 0.0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * lights[i].color;
        }

        // Reflektor (spotlight)
        if (lights[i].type == 2) {
            vec3 spotDir = normalize(lights[i].direction);
            float spot = dot(spotDir, -lightDir);
            if (spot < lights[i].spotCutoff) {
                diff = 0.0;
                specular = vec3(0.0);
            }
        }

        vec3 diffuse = diff * baseColor * lights[i].color;

        totalLight += (diffuse + specular) * attenuation;
    }

    out_Color = vec4(totalLight, 1.0);
}
