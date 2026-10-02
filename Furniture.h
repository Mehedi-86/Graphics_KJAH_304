#ifndef FURNITURE_H
#define FURNITURE_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// 1. Ceiling Fan: Anchored securely to ceiling, rotor and blades rotate smoothly
void drawCeilingFan(const glm::vec3 &pos, float bladeAngle, GLuint modelLoc, GLuint colorLoc);

// 2. Almirah: Detailed structure with base, shell, top cornice, door panels, handles
void drawAlmirah(const glm::vec3 &pos, bool isWood, GLuint modelLoc, GLuint colorLoc);

// 3. Pillow: Naturally rounded appearance scaling sphere
void drawPillow(const glm::vec3 &pos, GLuint modelLoc, GLuint colorLoc);

// 4. Enhanced Bed: Posts, rails, slats, headboard, footboard, and layered bedding
void drawBed(const glm::vec3 &pos, GLuint modelLoc, GLuint colorLoc);

// 5. Study Table
void drawStudyTable(const glm::vec3 &pos, GLuint modelLoc, GLuint colorLoc);

// 6. Chair (ergonomically proportioned below desk level)
void drawChair(const glm::vec3 &pos, GLuint modelLoc, GLuint colorLoc);

// 7. Tea Table
void drawTeaTable(const glm::vec3 &pos, GLuint modelLoc, GLuint colorLoc);

#endif // FURNITURE_H
