#include "Room.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// Room Structure: Walls, floor, ceiling, framed doors with recessed
// panels/handles, multi-pane cross-frame windows, and open balcony doorway.
void drawRoom(float balconyDoorAngle, float mainDoorAngle, GLuint modelLoc, GLuint colorLoc) {
  // Floor
  mat4 floor = translate(mat4(1.0f), vec3(0.0f, -2.5f, 0.0f));
  floor = scale(floor, vec3(16.0f, 0.1f, 18.0f));
  drawCube(floor, vec3(0.65f, 0.65f, 0.65f), modelLoc, colorLoc);

  // Ceiling
  mat4 ceil = translate(mat4(1.0f), vec3(0.0f, 3.5f, 0.0f));
  ceil = scale(ceil, vec3(16.0f, 0.1f, 18.0f));
  drawCube(ceil, vec3(0.95f, 0.95f, 0.95f), modelLoc, colorLoc);

  // Back Wall (z = -9.0f) with Doorway Opening for Door 2 ("ROOM 304" Main Entrance at x = 5.0f):
  // Inner half facing Room 304 interior (z = -8.95f, thickness 0.10f, color white 0.85f):
  // Left wall section: from x = -8.0f to +4.24f
  mat4 bwLeft = translate(mat4(1.0f), vec3(-1.88f, 0.5f, -8.95f));
  bwLeft = scale(bwLeft, vec3(12.24f, 6.05f, 0.10f));
  drawCube(bwLeft, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Right wall section: from x = +5.76f to +8.0f
  mat4 bwRight = translate(mat4(1.0f), vec3(6.88f, 0.5f, -8.95f));
  bwRight = scale(bwRight, vec3(2.24f, 6.05f, 0.10f));
  drawCube(bwRight, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Top header section directly above Door 2 (from y = 1.58f to 3.55f, height 1.97f)
  mat4 bwTop = translate(mat4(1.0f), vec3(5.0f, 2.565f, -8.95f));
  bwTop = scale(bwTop, vec3(1.52f, 1.97f, 0.10f));
  drawCube(bwTop, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Polished threshold transition plate on floor in Door 2's entrance doorway
  mat4 threshMain = translate(mat4(1.0f), vec3(5.0f, -2.435f, -9.0f));
  threshMain = scale(threshMain, vec3(1.52f, 0.03f, 0.26f));
  drawCube(threshMain, vec3(0.38f, 0.32f, 0.28f), modelLoc, colorLoc);

  // Front Wall (z = 9.0f) with Doorway Opening for Door 1 (Balcony Door at x = -5.0f):
  // Inner half facing room interior (z = 8.95f, thickness 0.10f, color white 0.85f):
  // Left wall section: from x = -8.0f to -5.76f
  mat4 fwLeftIn = translate(mat4(1.0f), vec3(-6.88f, 0.5f, 8.95f));
  fwLeftIn = scale(fwLeftIn, vec3(2.24f, 6.0f, 0.10f));
  drawCube(fwLeftIn, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Right wall section: from x = -4.24f to +8.0f
  mat4 fwRightIn = translate(mat4(1.0f), vec3(1.88f, 0.5f, 8.95f));
  fwRightIn = scale(fwRightIn, vec3(12.24f, 6.0f, 0.10f));
  drawCube(fwRightIn, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Top header wall section directly above the balcony doorway (from y = 1.58f to 3.5f)
  mat4 fwTopIn = translate(mat4(1.0f), vec3(-5.0f, 2.54f, 8.95f));
  fwTopIn = scale(fwTopIn, vec3(1.52f, 1.92f, 0.10f));
  drawCube(fwTopIn, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  vec3 hallWallCol = vec3(0.82f, 0.78f, 0.70f);

  mat4 fwLeftOut = translate(mat4(1.0f), vec3(-6.88f, 0.5f, 9.05f));
  fwLeftOut = scale(fwLeftOut, vec3(2.24f, 6.0f, 0.10f));
  drawCube(fwLeftOut, hallWallCol, modelLoc, colorLoc);

  mat4 fwRightOut = translate(mat4(1.0f), vec3(1.88f, 0.5f, 9.05f));
  fwRightOut = scale(fwRightOut, vec3(12.24f, 6.0f, 0.10f));
  drawCube(fwRightOut, hallWallCol, modelLoc, colorLoc);

  mat4 fwTopOut = translate(mat4(1.0f), vec3(-5.0f, 2.54f, 9.05f));
  fwTopOut = scale(fwTopOut, vec3(1.52f, 1.92f, 0.10f));
  drawCube(fwTopOut, hallWallCol, modelLoc, colorLoc);

  // Left Wall (x = -8.0f)
  mat4 leftWall = translate(mat4(1.0f), vec3(-8.0f, 0.5f, 0.0f));
  leftWall = scale(leftWall, vec3(0.2f, 6.0f, 18.0f));
  drawCube(leftWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Right Wall (x = +8.0f)
  mat4 rightWall = translate(mat4(1.0f), vec3(8.0f, 0.5f, 0.0f));
  rightWall = scale(rightWall, vec3(0.2f, 6.0f, 18.0f));
  drawCube(rightWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // --- DOORS ---
  // Door 1 on Front Wall (Balcony Door, opens on 'O' key press)
  drawDoor(vec3(-5.0f, -0.5f, 8.9f), true, balconyDoorAngle, "BALCONY", modelLoc, colorLoc);

  // Door 2 on Back Wall (Main Corridor Entrance, opens on 'M' key press)
  drawDoor(vec3(5.0f, -0.5f, -9.0f), false, mainDoorAngle, "ROOM 304", modelLoc, colorLoc);

  // --- BALCONY ---
  // Render full scenic balcony outside Door 1
  drawBalcony(modelLoc, colorLoc);

  // --- CORRIDOR ---
  // Render university dormitory hallway corridor outside Door 2
  drawCorridor(modelLoc, colorLoc);

  // 2 Multi-Pane Windows with Casing, Projecting Sill, and Cross Frame
  // Window 1 on Left Wall
  drawWindow(vec3(-7.9f, 1.0f, -3.0f), true, modelLoc, colorLoc);

  // Window 2 on Right Wall
  drawWindow(vec3(7.9f, 1.0f, -3.0f), false, modelLoc, colorLoc);
}
