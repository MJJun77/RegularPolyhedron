#version 330 core

in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    vec3 ambientLight = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    vec3 result = (ambientLight + diffuseLight) * objectColor;
    FragColor = vec4(result, 1.0);

}
