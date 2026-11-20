#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;

out vec4 out_Color;

vec3 resolveBaseColor(vec3 objColor, vec3 vertexColor)
{
    return (objColor == vec3(0.0)) ? vertexColor : objColor;
}

vec3 calcAmbient(vec3 baseColor, float strength)
{
    return strength * baseColor;
}

vec3 calcDiffuse(vec3 normal, vec3 lightDir, vec3 baseColor)
{
    float diff = max(dot(normal, lightDir), 0.0);
    return diff * baseColor;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, float shininess)
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    return spec * vec3(1.0);
}

vec3 calcBlinnPhong(vec3 fragPos, vec3 normal, vec3 baseColor)
{
    vec3 lightDir = normalize(lightPosition - fragPos);
    vec3 viewDir = normalize(viewPosition - fragPos);

    vec3 ambient = calcAmbient(baseColor, 0.1);
    vec3 diffuse = calcDiffuse(normal, lightDir, baseColor);
    vec3 specular = calcSpecular(normal, lightDir, viewDir, 32.0);

    return ambient + diffuse + specular;
}

void main(void)
{
    vec3 baseColor = resolveBaseColor(objectColor, fragColor);
    vec3 normal = normalize(worldNormal);
    vec3 result = calcBlinnPhong(vec3(worldPosition), normal, baseColor);

    out_Color = vec4(result, 1.0);
}