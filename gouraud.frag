#version 330 core
// =======================================================================
// GOURAUD FRAGMENT SHADER (Per-Vertex Illumination Model)
// Receives the hardware-interpolated lighting color computed per-vertex
// and simply assigns it to the fragment without per-pixel recomputation.
// =======================================================================
out vec4 FragColor;

in vec3 GouraudColor;

void main()
{
    FragColor = vec4(GouraudColor, 1.0);
}
