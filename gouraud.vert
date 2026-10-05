#version 330 core
// =======================================================================
// GOURAUD VERTEX SHADER (Per-Vertex Illumination Model)
// All ambient, diffuse, and specular lighting calculations are evaluated
// at every vertex, and the resulting color is passed down to be linearly
// interpolated across fragments during hardware rasterization.
// =======================================================================
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 GouraudColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 objectColor;
uniform float shininess;
uniform float specularStrength;
uniform vec3 viewPos;
uniform int isEmissive;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 5
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcGouraudDir(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 ambient = light.ambient * objectColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * objectColor;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularStrength);
    return ambient + diffuse + specular;
}

vec3 CalcGouraudPoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 ambient = light.ambient * objectColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * objectColor;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularStrength);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    if (isEmissive == 1) {
        GouraudColor = objectColor;
    } else {
        vec3 result = CalcGouraudDir(dirLight, normal, viewDir);
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            result += CalcGouraudPoint(pointLights[i], normal, fragPos, viewDir);
        }
        GouraudColor = result;
    }

    gl_Position = proj * view * vec4(fragPos, 1.0);
}
