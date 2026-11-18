#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

struct Light {
    int type;           // 0 = point, 1 = directional
    vec3 position;
    vec3 direction;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
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

    // Velmi jemné noční ambientní světlo (temná modrá noc)
    vec3 ambient = vec3(0.015, 0.02, 0.035) * baseColor;
    vec3 totalLight = ambient;

    for (int i = 0; i < numLights; ++i) {
        vec3 lightToPos = lights[i].position - vec3(worldPosition);
        float distance = length(lightToPos);
        vec3 lightDir = normalize(lightToPos);

        // Kvadratický útlum pro bodové světlo
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));

        // dif složka
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * baseColor * lights[i].color;

        // spek složka
        vec3 specular = vec3(0.0);
        if (diff > 0.0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
            vec3 specularColor = (objectColor == vec3(0.0)) ? vec3(0.3) : vec3(0.8);
            specular = spec * lights[i].color * specularColor;
        }

        totalLight += (diffuse + specular) * attenuation;
    }

    out_Color = vec4(clamp(totalLight, 0.0, 1.0),1.0);
}
