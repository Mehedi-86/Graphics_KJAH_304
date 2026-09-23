#ifndef PROPS_H
#define PROPS_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// 1. PC & Desk Setup: LCD monitor, bezel, stand, keyboard, mouse/pad, CPU tower
void drawComputer(const glm::vec3 &pos, float rotationY, GLuint modelLoc,
                  GLuint colorLoc);

// 2. Modern Dual-Band Wi-Fi Router: chassis, LED indicator strip, 4 antennas
void drawRouter(const glm::vec3 &pos, float rotationY, GLuint modelLoc,
                GLuint colorLoc);

// 3. Under-Bed Trolley Bag: clamshell body, ribs, zipper, corners, handle, wheels
void drawTrolleyBag(const glm::vec3 &pos, const glm::vec3 &shellColor,
                    GLuint modelLoc, GLuint colorLoc);

// 4. Tea Table Tableware: plate with raised rim, pastry, tinted beverage glass
void drawTableware(const glm::vec3 &pos, bool isVariant, GLuint modelLoc,
                   GLuint colorLoc);

#endif // PROPS_H
