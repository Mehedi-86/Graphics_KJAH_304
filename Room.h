#ifndef ROOM_H
#define ROOM_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// 1. Architectural Door: Jamb/frame, wood slab, 4 recessed panels, brass knob
void drawDoor(const glm::vec3 &pos, bool facingInwardZ, GLuint modelLoc,
              GLuint colorLoc);

// 2. Multi-Pane Window: Casing, projecting sill, glass pane, mullions & transom
void drawWindow(const glm::vec3 &pos, bool isLeftWall, GLuint modelLoc,
                GLuint colorLoc);

// 3. Wall Tube Light: Flush mounting base, two end caps, glowing tube
void drawTubeLight(const glm::vec3 &pos, int wallIndex, GLuint modelLoc,
                   GLuint colorLoc);

// 4. Room Shell: Floor, ceiling, 4 walls, doors, and windows
void drawRoom(GLuint modelLoc, GLuint colorLoc);

#endif // ROOM_H
