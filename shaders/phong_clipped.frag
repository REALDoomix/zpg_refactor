#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;

out vec4 out_Color;

void main(void){
    vec3 baseColor = (objectColor == vec3(0.0)) ? fragColor : objectColor;
    vec3 normal = normalize(worldNormal);

    vec3 lightDir = normalize(lightPosition - vec3(worldPosition));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * baseColor;

    vec3 viewDir = normalize(viewPosition - vec3(worldPosition));
    vec3 reflectDir = reflect(-lightDir, normal);

    // OPRAVA (b): Ořezání přesvitu na odvrácené straně.
    // Zrcadlová složka se smí počítat POUZE, pokud světlo dopadá na lícovou stranu (dot(normal, lightDir) > 0.0).
    float spec = 0.0;
    if (dot(normal, lightDir) > 0.0) {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    }
    vec3 specular = spec * vec3(1.0);

    vec3 ambient = 0.05 * baseColor;

    out_Color = vec4(ambient + diffuse + specular, 1.0);
}
