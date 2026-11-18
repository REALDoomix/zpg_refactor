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

uniform vec3 lightPosition; // fallback

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(textureUnitID, uv);
    if (texColor.a < 0.1) discard;

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPosition - worldPos);

    // Vyvazeny ambient pro jemne prosvetlene stiny v mistnosti
    vec3 ambient = vec3(0.15) * texColor.rgb;

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    int count = numLights;
    if (count <= 0) {
        vec3 L = normalize(lightPosition - worldPos);
        float diff = max(dot(N, L), 0.0);
        vec3 H = normalize(L + V);
        float spec = (diff > 0.0) ? pow(max(dot(N, H), 0.0), 32.0) : 0.0;
        totalDiffuse += diff * vec3(0.6);
        totalSpecular += spec * vec3(0.05);
    } else {
        for (int i = 0; i < count; ++i) {
            vec3 L;
            float attenuation = 1.0;
            if (lights[i].type == 1) {
                L = normalize(-lights[i].direction);
            } else {
                L = normalize(lights[i].position - worldPos);
                float dist = length(lights[i].position - worldPos);
                attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * (dist * dist));
            }

            float diff = max(dot(N, L), 0.0);
            vec3 H = normalize(L + V);
            // Jemny realisticky odlesk pro matne povrchy ucebny (stoly, tabule, omitka)
            float spec = (diff > 0.0) ? pow(max(dot(N, H), 0.0), 32.0) : 0.0;

            if (lights[i].type == 2) {
                vec3 spotDir = normalize(lights[i].direction);
                float spot = dot(spotDir, -L);
                if (spot < lights[i].spotCutoff) {
                    diff = 0.0;
                    spec = 0.0;
                }
            }

            totalDiffuse += diff * lights[i].color * attenuation;
            totalSpecular += spec * lights[i].color * attenuation * 0.08;
        }
    }

    // Difuzni slozka omezena tak, aby nedochazelo k presvetleni a ztrate kresby textur
    vec3 diffuse = 0.40 * totalDiffuse * texColor.rgb;
    vec3 result = ambient + diffuse + totalSpecular;

    // Zabraneni prepalu
    result = clamp(result, 0.0, 1.0);

    FragColor = vec4(result, texColor.a);
}
