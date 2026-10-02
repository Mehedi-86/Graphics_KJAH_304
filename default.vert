#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // Compute world-space fragment position
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform vertex normal to world space using the normal matrix
    // Normal matrix = transpose(inverse(model)) to handle non-uniform scaling correctly
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = proj * view * vec4(FragPos, 1.0);
}