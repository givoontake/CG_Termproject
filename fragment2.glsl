#version 330 core

in vec3 out_Color;

out vec4 fragColor;
in vec3 fragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
float ambientLight = 0.3;
vec3 ambient = ambientLight * lightColor;


vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - fragPos);
float diffuseLight = max(dot (norm, lightDir), 0.0);
vec3 diffuse = diffuseLight * lightColor;

int shininess = 128;
vec3 viewDir = normalize(viewPos - fragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float specularLight = max(dot (viewDir, reflectDir), 0.0);
specularLight = pow(specularLight, shininess);
vec3 specular = specularLight * lightColor;

vec3 result = (ambient + diffuse + specular) * objectColor;

fragColor = vec4(result, 1.0);

}
