#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// Initialize all primitive meshes (Cube, Sphere, Cylinder) with vertex normals
void initPrimitives();

// Free OpenGL buffer resources allocated for primitive meshes
void cleanupPrimitives();

// Set uniform location for isEmissive
void setEmissiveUniformLoc(GLint loc);

// Enable or disable self-illumination (emissive mode for sun, sky, lamps)
void setEmissive(bool emissive);

// General draw helper
void drawMesh(GLuint vao, GLsizei count, const glm::mat4 &model,
              const glm::vec3 &color, GLuint modelLoc, GLuint colorLoc);

// Specific primitive draw calls
void drawCube(const glm::mat4 &model, const glm::vec3 &color, GLuint modelLoc,
              GLuint colorLoc);

void drawSphere(const glm::mat4 &model, const glm::vec3 &color, GLuint modelLoc,
                GLuint colorLoc);

void drawCylinder(const glm::mat4 &model, const glm::vec3 &color, GLuint modelLoc,
                  GLuint colorLoc);

#endif // PRIMITIVES_H
