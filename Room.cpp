#include "Room.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 1. Detailed Architectural Door:
// Surrounding jamb/frame, rich wood slab, 4 recessed decorative panels, and
// metallic doorknob.
void drawDoor(const vec3 &pos, bool facingInwardZ, GLuint modelLoc,
              GLuint colorLoc) {
  vec3 frameCol = vec3(0.32f, 0.16f, 0.06f);
  vec3 doorCol = vec3(0.48f, 0.26f, 0.12f);
  vec3 panelCol = vec3(0.36f, 0.19f, 0.08f);
  vec3 knobCol = vec3(0.90f, 0.80f, 0.38f);

  // Left jamb
  mat4 lJamb = translate(mat4(1.0f), pos + vec3(-0.76f, 0.0f, 0.0f));
  lJamb = scale(lJamb, vec3(0.12f, 4.08f, 0.24f));
  drawCube(lJamb, frameCol, modelLoc, colorLoc);

  // Right jamb
  mat4 rJamb = translate(mat4(1.0f), pos + vec3(0.76f, 0.0f, 0.0f));
  rJamb = scale(rJamb, vec3(0.12f, 4.08f, 0.24f));
  drawCube(rJamb, frameCol, modelLoc, colorLoc);

  // Top header casing
  mat4 hJamb = translate(mat4(1.0f), pos + vec3(0.0f, 2.02f, 0.0f));
  hJamb = scale(hJamb, vec3(1.64f, 0.12f, 0.24f));
  drawCube(hJamb, frameCol, modelLoc, colorLoc);

  // Main door slab
  mat4 slab = translate(mat4(1.0f), pos + vec3(0.0f, -0.04f, 0.0f));
  slab = scale(slab, vec3(1.42f, 3.96f, 0.12f));
  drawCube(slab, doorCol, modelLoc, colorLoc);

  // 4 Recessed Door Panels
  float panZ = facingInwardZ ? (pos.z - 0.055f) : (pos.z + 0.055f);
  float panX[2] = {pos.x - 0.35f, pos.x + 0.35f};

  for (int x = 0; x < 2; x++) {
    // Upper vertical panel
    mat4 uPanel = translate(mat4(1.0f), vec3(panX[x], pos.y + 0.95f, panZ));
    uPanel = scale(uPanel, vec3(0.48f, 1.35f, 0.025f));
    drawCube(uPanel, panelCol, modelLoc, colorLoc);

    // Lower vertical panel
    mat4 dPanel = translate(mat4(1.0f), vec3(panX[x], pos.y - 0.75f, panZ));
    dPanel = scale(dPanel, vec3(0.48f, 1.25f, 0.025f));
    drawCube(dPanel, panelCol, modelLoc, colorLoc);
  }

  // Metallic brass doorknob
  float knobX = facingInwardZ ? (pos.x + 0.52f) : (pos.x - 0.52f);
  float knobZ = facingInwardZ ? (pos.z - 0.075f) : (pos.z + 0.075f);

  // Rosette plate
  mat4 rosette = translate(mat4(1.0f), vec3(knobX, pos.y, knobZ));
  rosette = scale(rosette, vec3(0.09f, 0.09f, 0.015f));
  drawCube(rosette, knobCol, modelLoc, colorLoc);

  // Knob sphere
  mat4 knobSphere = translate(
      mat4(1.0f), vec3(knobX, pos.y, knobZ + (facingInwardZ ? -0.04f : 0.04f)));
  knobSphere = scale(knobSphere, vec3(0.045f, 0.045f, 0.045f));
  drawSphere(knobSphere, knobCol * 1.05f, modelLoc, colorLoc);
}

// 2. Multi-Pane Window:
// Outer casing, projecting window sill, tinted glass, and multi-pane cross
// frame (mullions/transom).
void drawWindow(const vec3 &pos, bool isLeftWall, GLuint modelLoc,
                GLuint colorLoc) {
  vec3 frameCol = vec3(0.38f, 0.20f, 0.09f);
  vec3 sillCol = vec3(0.44f, 0.23f, 0.11f);
  vec3 glassCol = vec3(0.46f, 0.74f, 0.92f);

  // Top header frame
  mat4 topF = translate(mat4(1.0f), pos + vec3(0.0f, 1.05f, 0.0f));
  topF = scale(topF, vec3(0.24f, 0.12f, 2.22f));
  drawCube(topF, frameCol, modelLoc, colorLoc);

  // Bottom frame
  mat4 botF = translate(mat4(1.0f), pos + vec3(0.0f, -1.05f, 0.0f));
  botF = scale(botF, vec3(0.24f, 0.12f, 2.22f));
  drawCube(botF, frameCol, modelLoc, colorLoc);

  // Left jamb frame
  mat4 leftF = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, -1.05f));
  leftF = scale(leftF, vec3(0.24f, 2.00f, 0.12f));
  drawCube(leftF, frameCol, modelLoc, colorLoc);

  // Right jamb frame
  mat4 rightF = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, 1.05f));
  rightF = scale(rightF, vec3(0.24f, 2.00f, 0.12f));
  drawCube(rightF, frameCol, modelLoc, colorLoc);

  // Projecting interior window sill
  float sillX = isLeftWall ? (pos.x + 0.06f) : (pos.x - 0.06f);
  mat4 sill = translate(mat4(1.0f), vec3(sillX, pos.y - 1.11f, pos.z));
  sill = scale(sill, vec3(0.36f, 0.08f, 2.40f));
  drawCube(sill, sillCol, modelLoc, colorLoc);

  // Sky blue glass pane
  mat4 glass = translate(mat4(1.0f), pos);
  glass = scale(glass, vec3(0.08f, 2.00f, 2.00f));
  drawCube(glass, glassCol, modelLoc, colorLoc);

  // Multi-pane cross frame over the glass:
  // Central vertical mullion bar
  mat4 mullion = translate(mat4(1.0f), pos);
  mullion = scale(mullion, vec3(0.14f, 2.00f, 0.08f));
  drawCube(mullion, frameCol, modelLoc, colorLoc);

  // Central horizontal transom bar
  mat4 transom = translate(mat4(1.0f), pos);
  transom = scale(transom, vec3(0.14f, 0.08f, 2.00f));
  drawCube(transom, frameCol, modelLoc, colorLoc);
}

// 3. Tube Lights:
// Realistic fluorescent fixture: slim metallic mounting base flush to the wall,
// two end-caps/sockets, and a glowing off-white cylindrical tube held between
// them.
void drawTubeLight(const vec3 &pos, int wallIndex, GLuint modelLoc,
                   GLuint colorLoc) {
  vec3 metalChassis = vec3(0.65f, 0.67f, 0.70f);
  vec3 socketCol = vec3(0.30f, 0.30f, 0.33f);
  vec3 glowingTubeCol = vec3(0.98f, 0.99f, 0.94f);

  if (wallIndex < 2) {
    // Walls running along X (Back wall: index 0, Front wall: index 1)
    // Metallic mounting plate flush to wall
    mat4 basePlate = translate(mat4(1.0f), pos);
    basePlate = scale(basePlate, vec3(3.20f, 0.14f, 0.035f));
    drawCube(basePlate, metalChassis, modelLoc, colorLoc);

    // Two end caps
    float capX[2] = {-1.45f, 1.45f};
    float zOff = (wallIndex == 0) ? 0.035f : -0.035f;
    for (int c = 0; c < 2; c++) {
      mat4 cap = translate(mat4(1.0f), pos + vec3(capX[c], 0.0f, zOff));
      cap = scale(cap, vec3(0.08f, 0.12f, 0.07f));
      drawCube(cap, socketCol, modelLoc, colorLoc);
    }

    // Glowing off-white cylindrical tube (cylinder oriented along X)
    mat4 tube = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, zOff * 1.05f));
    tube = rotate(tube, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    tube = scale(tube, vec3(0.032f, 2.80f, 0.032f));
    drawCylinder(tube, glowingTubeCol, modelLoc, colorLoc);
  } else {
    // Walls running along Z (Left wall: index 2, Right wall: index 3)
    // Metallic mounting plate flush to wall
    mat4 basePlate = translate(mat4(1.0f), pos);
    basePlate = scale(basePlate, vec3(0.035f, 0.14f, 3.20f));
    drawCube(basePlate, metalChassis, modelLoc, colorLoc);

    // Two end caps
    float capZ[2] = {-1.45f, 1.45f};
    float xOff = (wallIndex == 2) ? 0.035f : -0.035f;
    for (int c = 0; c < 2; c++) {
      mat4 cap = translate(mat4(1.0f), pos + vec3(xOff, 0.0f, capZ[c]));
      cap = scale(cap, vec3(0.07f, 0.12f, 0.08f));
      drawCube(cap, socketCol, modelLoc, colorLoc);
    }

    // Glowing off-white cylindrical tube (cylinder oriented along Z)
    mat4 tube = translate(mat4(1.0f), pos + vec3(xOff * 1.05f, 0.0f, 0.0f));
    tube = rotate(tube, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    tube = scale(tube, vec3(0.032f, 2.80f, 0.032f));
    drawCylinder(tube, glowingTubeCol, modelLoc, colorLoc);
  }
}

// 4. Room Structure: Walls, floor, ceiling, framed doors with recessed
// panels/handles, and multi-pane cross-frame windows.
void drawRoom(GLuint modelLoc, GLuint colorLoc) {
  // Floor
  mat4 floor = translate(mat4(1.0f), vec3(0.0f, -2.5f, 0.0f));
  floor = scale(floor, vec3(16.0f, 0.1f, 18.0f));
  drawCube(floor, vec3(0.65f, 0.65f, 0.65f), modelLoc, colorLoc);

  // Ceiling
  mat4 ceil = translate(mat4(1.0f), vec3(0.0f, 3.5f, 0.0f));
  ceil = scale(ceil, vec3(16.0f, 0.1f, 18.0f));
  drawCube(ceil, vec3(0.95f, 0.95f, 0.95f), modelLoc, colorLoc);

  // Back Wall
  mat4 backWall = translate(mat4(1.0f), vec3(0.0f, 0.5f, -9.0f));
  backWall = scale(backWall, vec3(16.0f, 6.0f, 0.2f));
  drawCube(backWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Front Wall
  mat4 frontWall = translate(mat4(1.0f), vec3(0.0f, 0.5f, 9.0f));
  frontWall = scale(frontWall, vec3(16.0f, 6.0f, 0.2f));
  drawCube(frontWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Left Wall
  mat4 leftWall = translate(mat4(1.0f), vec3(-8.0f, 0.5f, 0.0f));
  leftWall = scale(leftWall, vec3(0.2f, 6.0f, 18.0f));
  drawCube(leftWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // Right Wall
  mat4 rightWall = translate(mat4(1.0f), vec3(8.0f, 0.5f, 0.0f));
  rightWall = scale(rightWall, vec3(0.2f, 6.0f, 18.0f));
  drawCube(rightWall, vec3(0.85f, 0.85f, 0.85f), modelLoc, colorLoc);

  // 2 Detailed Architectural Doors with Jamb, Panels, and Metallic Doorknob
  // Door 1 on Front Wall (faces room towards -Z)
  drawDoor(vec3(-5.0f, -0.5f, 8.9f), true, modelLoc, colorLoc);

  // Door 2 on Back Wall (faces room towards +Z)
  drawDoor(vec3(5.0f, -0.5f, -8.9f), false, modelLoc, colorLoc);

  // 2 Multi-Pane Windows with Casing, Projecting Sill, and Cross Frame
  // Window 1 on Left Wall
  drawWindow(vec3(-7.9f, 1.0f, -3.0f), true, modelLoc, colorLoc);

  // Window 2 on Right Wall
  drawWindow(vec3(7.9f, 1.0f, -3.0f), false, modelLoc, colorLoc);
}
