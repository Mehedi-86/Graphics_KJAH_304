#ifndef ROOM_H
#define ROOM_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// 1. Architectural Door: Jamb/frame, opening wood slab with hinges, 4 recessed panels, brass knob
void drawDoor(const glm::vec3 &pos, bool facingInwardZ, float openAngle, const char* label,
              GLuint modelLoc, GLuint colorLoc);

// 2. Multi-Pane Window: Casing, projecting sill, glass pane, mullions & transom
void drawWindow(const glm::vec3 &pos, bool isLeftWall, GLuint modelLoc,
                GLuint colorLoc);

// 3. Wall Tube Light: Flush mounting base, two end caps, glowing/unlit tube depending on isOn
void drawTubeLight(const glm::vec3 &pos, int wallIndex, GLuint modelLoc,
                   GLuint colorLoc, bool isOn = true);

// 4. Balcony: outdoor terrace flooring, balustrades/railings, outdoor furniture, planters, vista
void drawBalcony(GLuint modelLoc, GLuint colorLoc);

// 5. Room Shell: Floor, ceiling, 4 walls (with balcony doorway opening), doors, and windows
void drawRoom(float balconyDoorAngle, GLuint modelLoc, GLuint colorLoc);

// 6. Wall Switch Board: 4-gang electrical switch board on room wall with toggles & LED status indicators
void drawSwitchBoard(const glm::vec3 &pos, bool s1, bool s2, bool s3, bool s4,
                     GLuint modelLoc, GLuint colorLoc);

#endif // ROOM_H
