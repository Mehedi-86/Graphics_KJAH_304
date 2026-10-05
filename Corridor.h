#ifndef CORRIDOR_H
#define CORRIDOR_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// University Dormitory Hallway Corridor:
// Enclosed hallway outside Door 304 with floor, ceiling, opposite doors ("ROOM 303", "ROOM 305"),
// adjacent doors ("ROOM 302", "ROOM 306"), notice board, fire extinguisher, water cooler,
// illuminated exit sign, and fluorescent ceiling lights.
void drawCorridor(GLuint modelLoc, GLuint colorLoc);

#endif // CORRIDOR_H
