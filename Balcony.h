#ifndef BALCONY_H
#define BALCONY_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// 1. Wing Balcony: Architectural standard dorm balcony on hall wings matching Room 304's styling
void drawWingBalcony(const glm::vec3 &center, float width, float depth, int dir,
                     GLuint modelLoc, GLuint colorLoc, int flowerSeed = 0);

// 2. Balcony Wooden Door: Simple collegiate solid wooden door with outer casing, threshold sill, 4 recessed wooden panels, and brass knob (no glass)
void drawBalconyDoor(const glm::vec3 &wallPos, float width, float height, int dir,
                     GLuint modelLoc, GLuint colorLoc);

// 3. Balcony: outdoor terrace flooring, balustrades/railings, outdoor furniture, planters, vista
void drawBalcony(GLuint modelLoc, GLuint colorLoc);

#endif // BALCONY_H
