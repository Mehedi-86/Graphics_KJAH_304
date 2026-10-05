#ifndef ROOM_H
#define ROOM_H

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>

// Modular architectural components:
// 1. Fixtures: Doors, Windows, Tube Lights, Switchboard
#include "Fixtures.h"
// 2. Balcony: Terrace, Railings, Courtyard Vista, Wing Balconies
#include "Balcony.h"
// 3. Corridor: Hallway outside Room 304, Opposite/Adjacent Doors, Notice Board, Fire Extinguisher
#include "Corridor.h"

// 4. Room Shell: Floor, ceiling, 4 walls (with balcony and corridor doorway openings),
// and composition calling doors, windows, balcony, and corridor
void drawRoom(float balconyDoorAngle, float mainDoorAngle, GLuint modelLoc, GLuint colorLoc);

#endif // ROOM_H
