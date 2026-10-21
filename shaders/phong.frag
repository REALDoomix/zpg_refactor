in vec4 worldPosition;
in vec3 worldNormal;
in vec3 fragColor;

uniform vec3 lightPos;

out vec4 out_Color;


void main(void){
    //vec3 lightPosition= vec3(10.0,10.0,10.0); //Point Light 

    vec3 lightToVector = lightPos - vec3(worldPosition);
    float dotProduct = max(dot(normalize(lightToVector), normalize(worldNormal)), 0.0);
    vec3 diffuse = dotProduct * fragColor;
    vec3 ambient = 0.1 * fragColor;

    out_Color = vec4(ambient + diffuse, 1.0);
}