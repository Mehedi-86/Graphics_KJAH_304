#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 GouraudColor;

// Shading mode: 0 = Phong Shading (Per-pixel), 1 = Gouraud Shading (Per-vertex)
uniform int useGouraud;

// Surface material properties
uniform vec3 objectColor;
uniform float shininess;          // Specular shininess exponent (e.g. 32.0)
uniform float specularStrength;   // Specular intensity factor (e.g. 0.3)
uniform int isEmissive;           // 1 for self-illuminating objects (sky, sun, bulbs), 0 for lit surfaces

// Camera/Eye position in world coordinates for specular calculations
uniform vec3 viewPos;

// Directional Light (Sunlight / Outdoor sky illumination)
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

// Point Light (4 Indoor fluorescent room lights & 1 Outdoor balcony coach light)
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 6
uniform PointLight pointLights[NR_POINT_LIGHTS];

// ============================================================
// MANUAL PHONG LIGHTING CALCULATIONS
// ============================================================

// Computes directional light contribution (Sun) using Phong model
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // 1. Ambient component
    vec3 ambient = light.ambient * objectColor;

    // 2. Diffuse component (Lambert's Cosine Law: N . L)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * objectColor;

    // 3. Specular component (Phong Reflection: (R . V)^shininess)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularStrength);

    return (ambient + diffuse + specular);
}

// Computes point light contribution with distance attenuation
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // 1. Ambient component
    vec3 ambient = light.ambient * objectColor;

    // 2. Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * objectColor;

    // 3. Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specularStrength);

    // 4. Distance attenuation: 1.0 / (kc + kl * d + kq * d^2)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    // Self-illuminated objects (sun, sky, tube light bulbs) glow directly without shading
    if (isEmissive == 1) {
        FragColor = vec4(objectColor, 1.0);
        return;
    }

    // Gouraud Shading: Output linearly-interpolated per-vertex lighting color
    if (useGouraud == 1) {
        FragColor = vec4(GouraudColor, 1.0);
        return;
    }

    // Phong Shading: Per-fragment normal interpolation and manual lighting computation
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Phase 1: Directional lighting (Outdoor sunlight)
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // Phase 2: Point lights (4 Indoor ceiling lights + Balcony lantern)
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}