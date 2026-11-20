#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D textureUnitID;
uniform vec3 viewPosition;

struct Light {
    vec3 position;
    vec3 color;
    int type; // 0 = point, 1 = directional, 2 = spotlight
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float spotCutoff;
};

#define MAX_LIGHTS 8
uniform int numLights;
uniform Light lights[MAX_LIGHTS];
uniform vec3 lightPosition; // fallback for single light

out vec4 FragColor;

float calcAttenuation(Light light, vec3 fragPos)
{
    float dist = length(light.position - fragPos);
    return 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
}

bool isInsideSpotCone(Light light, vec3 lightDir)
{
    vec3 spotDir = normalize(light.direction);
    float spot = dot(spotDir, -lightDir);
    return spot >= light.spotCutoff;
}

void calcBlinnPhong(vec3 N, vec3 V, vec3 L, vec3 lightColor, float attenuation,
                    inout vec3 diffuseAcc, inout vec3 specularAcc)
{
    float diff = max(dot(N, L), 0.0);
    vec3 H = normalize(L + V);
    float spec = (diff > 0.0) ? pow(max(dot(N, H), 0.0), 32.0) : 0.0;

    diffuseAcc += diff * lightColor * attenuation;
    specularAcc += spec * lightColor * attenuation * 0.3;
}

void evaluateLight(Light light, vec3 N, vec3 V, vec3 fragPos,
                   inout vec3 diffuseAcc, inout vec3 specularAcc)
{
    vec3 L;
    float attenuation = 1.0;

    if (light.type == 1) {
        L = normalize(-light.direction);
    } else {
        L = normalize(light.position - fragPos);
        attenuation = calcAttenuation(light, fragPos);
    }

    if (light.type == 2 && !isInsideSpotCone(light, L)) {
        return;
    }

    calcBlinnPhong(N, V, L, light.color, attenuation, diffuseAcc, specularAcc);
}

void main()
{
    vec4 texColor = texture(textureUnitID, uv);
    if (texColor.a < 0.1) discard;

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPosition - worldPos);

    vec3 ambient = vec3(0.25) * texColor.rgb;
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    int count = min(numLights, MAX_LIGHTS);
    if (count <= 0) {
        vec3 L = normalize(lightPosition - worldPos);
        calcBlinnPhong(N, V, L, vec3(1.0), 1.0, totalDiffuse, totalSpecular);
    } else {
        for (int i = 0; i < count; ++i) {
            evaluateLight(lights[i], N, V, worldPos, totalDiffuse, totalSpecular);
        }
    }

    vec3 result = ambient + totalDiffuse * texColor.rgb + totalSpecular;
    FragColor = vec4(result, texColor.a);
}