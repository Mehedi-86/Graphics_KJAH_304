#include "Fixtures.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 1. Detailed Architectural Door:
// Surrounding fixed jamb/frame, opening rich wood slab swinging on hinges,
// 4 recessed decorative panels, metallic doorknob, and an engraved room/balcony plaque.
void drawDoor(const vec3 &pos, bool facingInwardZ, float openAngle, const char* label,
              GLuint modelLoc, GLuint colorLoc) {
  vec3 frameCol = vec3(0.32f, 0.16f, 0.06f);
  vec3 doorCol = vec3(0.48f, 0.26f, 0.12f);
  vec3 panelCol = vec3(0.36f, 0.19f, 0.08f);
  vec3 knobCol = vec3(0.90f, 0.80f, 0.38f);

  // --- FIXED CASING & FRAME ---
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

  // Door sign / plaque mounted on top casing
  if (label != nullptr) {
    float plaqueZ = facingInwardZ ? (pos.z - 0.125f) : (pos.z + 0.125f);
    mat4 plaqueBase = translate(mat4(1.0f), vec3(pos.x, pos.y + 2.22f, plaqueZ));
    plaqueBase = scale(plaqueBase, vec3(0.92f, 0.22f, 0.035f));
    drawCube(plaqueBase, vec3(0.18f, 0.14f, 0.10f), modelLoc, colorLoc);

    mat4 plaqueFace = translate(mat4(1.0f), vec3(pos.x, pos.y + 2.22f, plaqueZ + (facingInwardZ ? -0.015f : 0.015f)));
    plaqueFace = scale(plaqueFace, vec3(0.84f, 0.16f, 0.012f));
    vec3 plaqueFaceCol = facingInwardZ ? vec3(0.12f, 0.42f, 0.32f) : vec3(0.20f, 0.28f, 0.46f);
    drawCube(plaqueFace, plaqueFaceCol, modelLoc, colorLoc);

    // Decorative brass trim accents
    mat4 trimTop = translate(mat4(1.0f), vec3(pos.x, pos.y + 2.32f, plaqueZ + (facingInwardZ ? -0.012f : 0.012f)));
    trimTop = scale(trimTop, vec3(0.88f, 0.018f, 0.020f));
    drawCube(trimTop, knobCol, modelLoc, colorLoc);

    mat4 trimBot = translate(mat4(1.0f), vec3(pos.x, pos.y + 2.12f, plaqueZ + (facingInwardZ ? -0.012f : 0.012f)));
    trimBot = scale(trimBot, vec3(0.88f, 0.018f, 0.020f));
    drawCube(trimBot, knobCol, modelLoc, colorLoc);
  }

  // --- HINGED MOVING DOOR SLAB ---
  // The door leaf pivots at the left jamb (x = pos.x - 0.70f)
  mat4 hinge = translate(mat4(1.0f), vec3(pos.x - 0.70f, pos.y, pos.z));
  if (facingInwardZ) {
    // Door on front wall: swings inward into the room (-Z direction)
    hinge = rotate(hinge, radians(-openAngle), vec3(0.0f, 1.0f, 0.0f));
  } else {
    // Door on back wall: swings inward into the room (+Z direction)
    hinge = rotate(hinge, radians(openAngle), vec3(0.0f, 1.0f, 0.0f));
  }

  // Raised bottom clearance to y = -2.43f (2cm above floor at -2.45f)
  mat4 slab = hinge * translate(mat4(1.0f), vec3(0.70f, 0.005f, 0.0f));
  slab = scale(slab, vec3(1.40f, 3.87f, 0.10f));
  drawCube(slab, doorCol, modelLoc, colorLoc);

  // 4 Recessed molded panels (2 upper, 2 lower) on front & back faces
  float pX[2] = {0.35f, 1.05f};
  float pY[2] = {0.80f, -0.75f};
  float pZFace[2] = {0.051f, -0.051f};

  for (int f = 0; f < 2; f++) {
    for (int ix = 0; ix < 2; ix++) {
      for (int iy = 0; iy < 2; iy++) {
        mat4 panel = hinge * translate(mat4(1.0f), vec3(pX[ix], pY[iy] + 0.005f, pZFace[f]));
        panel = scale(panel, vec3(0.48f, 1.25f, 0.018f));
        drawCube(panel, panelCol, modelLoc, colorLoc);
      }
    }
  }

  // Doorknob & Keyhole Escutcheon plate
  for (int f = 0; f < 2; f++) {
    float kZ = (f == 0) ? 0.08f : -0.08f;
    mat4 plate = hinge * translate(mat4(1.0f), vec3(1.26f, -0.10f + 0.005f, (f == 0) ? 0.054f : -0.054f));
    plate = scale(plate, vec3(0.08f, 0.22f, 0.012f));
    drawCube(plate, knobCol * 0.85f, modelLoc, colorLoc);

    mat4 knobStem = hinge * translate(mat4(1.0f), vec3(1.26f, -0.05f + 0.005f, (f == 0) ? 0.065f : -0.065f));
    knobStem = scale(knobStem, vec3(0.03f, 0.03f, 0.04f));
    drawCylinder(knobStem, knobCol, modelLoc, colorLoc);

    mat4 knobBall = hinge * translate(mat4(1.0f), vec3(1.26f, -0.05f + 0.005f, kZ));
    knobBall = scale(knobBall, vec3(0.065f, 0.065f, 0.065f));
    drawSphere(knobBall, knobCol, modelLoc, colorLoc);
  }
}

// 2. Multi-Pane Window:
// Outer wood casing, projecting bottom sill, clear glass pane, and cross mullion bars.
void drawWindow(const vec3 &pos, bool isLeftWall, GLuint modelLoc,
                GLuint colorLoc) {
  vec3 casingCol = vec3(0.38f, 0.20f, 0.08f);
  vec3 glassCol = vec3(0.45f, 0.70f, 0.90f);
  vec3 sillCol = vec3(0.30f, 0.15f, 0.06f);

  float rotY = isLeftWall ? 90.0f : -90.0f;
  mat4 root = translate(mat4(1.0f), pos);
  root = rotate(root, radians(rotY), vec3(0.0f, 1.0f, 0.0f));

  // Projecting bottom sill
  mat4 sill = root * translate(mat4(1.0f), vec3(0.0f, -1.22f, 0.08f));
  sill = scale(sill, vec3(3.40f, 0.14f, 0.32f));
  drawCube(sill, sillCol, modelLoc, colorLoc);

  // Left & Right Outer Casing
  mat4 lCasing = root * translate(mat4(1.0f), vec3(-1.55f, 0.0f, 0.04f));
  lCasing = scale(lCasing, vec3(0.18f, 2.40f, 0.18f));
  drawCube(lCasing, casingCol, modelLoc, colorLoc);

  mat4 rCasing = root * translate(mat4(1.0f), vec3(1.55f, 0.0f, 0.04f));
  rCasing = scale(rCasing, vec3(0.18f, 2.40f, 0.18f));
  drawCube(rCasing, casingCol, modelLoc, colorLoc);

  // Top header casing
  mat4 tCasing = root * translate(mat4(1.0f), vec3(0.0f, 1.22f, 0.04f));
  tCasing = scale(tCasing, vec3(3.30f, 0.16f, 0.18f));
  drawCube(tCasing, casingCol, modelLoc, colorLoc);

  // Translucent tinted glass pane
  mat4 glass = root * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.01f));
  glass = scale(glass, vec3(2.95f, 2.25f, 0.02f));
  drawCube(glass, glassCol, modelLoc, colorLoc);

  // Vertical central mullion bar
  mat4 vBar = root * translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.025f));
  vBar = scale(vBar, vec3(0.08f, 2.25f, 0.05f));
  drawCube(vBar, casingCol, modelLoc, colorLoc);

  // Horizontal central transom bar
  mat4 hBar = root * translate(mat4(1.0f), vec3(0.0f, 0.15f, 0.025f));
  hBar = scale(hBar, vec3(2.95f, 0.08f, 0.05f));
  drawCube(hBar, casingCol, modelLoc, colorLoc);
}

// 3. Wall Tube Light:
// Flush mounting base, two end caps, glowing/unlit tube depending on isOn
void drawTubeLight(const vec3 &pos, int wallIndex, GLuint modelLoc,
                   GLuint colorLoc, bool isOn) {
  vec3 fixtureCol = vec3(0.85f, 0.85f, 0.88f);
  vec3 socketCol = vec3(0.20f, 0.20f, 0.22f);
  vec3 glowingTubeCol = vec3(1.0f, 1.0f, 0.95f);
  vec3 unlitTubeCol = vec3(0.55f, 0.55f, 0.52f);

  if (wallIndex == 0 || wallIndex == 1) {
    // Back / Front Wall: Oriented along X axis
    mat4 base = translate(mat4(1.0f), pos);
    base = scale(base, vec3(3.10f, 0.14f, 0.07f));
    drawCube(base, fixtureCol, modelLoc, colorLoc);

    // Two end caps
    float capX[2] = {-1.45f, 1.45f};
    float zOff = (wallIndex == 0) ? 0.035f : -0.035f;
    for (int c = 0; c < 2; c++) {
      mat4 cap = translate(mat4(1.0f), pos + vec3(capX[c], 0.0f, zOff));
      cap = scale(cap, vec3(0.08f, 0.12f, 0.07f));
      drawCube(cap, socketCol, modelLoc, colorLoc);
    }

    // Cylindrical tube (cylinder oriented along X)
    mat4 tube = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, zOff * 1.05f));
    tube = rotate(tube, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    tube = scale(tube, vec3(0.032f, 2.80f, 0.032f));
    if (isOn) {
      setEmissive(true);
      drawCylinder(tube, glowingTubeCol, modelLoc, colorLoc);
      setEmissive(false);
    } else {
      drawCylinder(tube, unlitTubeCol, modelLoc, colorLoc);
    }
  } else {
    // Left / Right Wall: Oriented along Z axis
    mat4 base = translate(mat4(1.0f), pos);
    base = scale(base, vec3(0.07f, 0.14f, 3.10f));
    drawCube(base, fixtureCol, modelLoc, colorLoc);

    // Two end caps
    float capZ[2] = {-1.45f, 1.45f};
    float xOff = (wallIndex == 2) ? 0.035f : -0.035f;
    for (int c = 0; c < 2; c++) {
      mat4 cap = translate(mat4(1.0f), pos + vec3(xOff, 0.0f, capZ[c]));
      cap = scale(cap, vec3(0.07f, 0.12f, 0.08f));
      drawCube(cap, socketCol, modelLoc, colorLoc);
    }

    // Cylindrical tube (cylinder oriented along Z)
    mat4 tube = translate(mat4(1.0f), pos + vec3(xOff * 1.05f, 0.0f, 0.0f));
    tube = rotate(tube, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    tube = scale(tube, vec3(0.032f, 2.80f, 0.032f));
    if (isOn) {
      setEmissive(true);
      drawCylinder(tube, glowingTubeCol, modelLoc, colorLoc);
      setEmissive(false);
    } else {
      drawCylinder(tube, unlitTubeCol, modelLoc, colorLoc);
    }
  }
}

// 4. Wall Switch Board: 4-gang electrical switch board on room wall with toggles & LED status indicators
void drawSwitchBoard(const vec3 &pos, bool s1, bool s2, bool s3, bool s4,
                     GLuint modelLoc, GLuint colorLoc) {
  vec3 plateCol = vec3(0.92f, 0.92f, 0.90f);        // Modern white/cream gang plate
  vec3 beveledBorderCol = vec3(0.78f, 0.78f, 0.80f); // Beveled border
  vec3 wellCol = vec3(0.24f, 0.24f, 0.26f);          // Recessed switch well
  vec3 switchOnCol = vec3(0.98f, 0.98f, 0.99f);      // Crisp white toggle
  vec3 switchOffCol = vec3(0.60f, 0.60f, 0.63f);     // Dimmed toggle
  vec3 ledOnCol = vec3(0.15f, 0.98f, 0.25f);         // Emerald green glowing LED
  vec3 ledOffCol = vec3(0.55f, 0.12f, 0.12f);        // Dim red indicator LED
  vec3 labelCol = vec3(0.25f, 0.25f, 0.28f);         // Dark label

  // 1. Outer Beveled Mounting Baseplate flush to wall
  mat4 basePlate = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, 0.005f))
                   * scale(mat4(1.0f), vec3(0.64f, 0.34f, 0.02f));
  drawCube(basePlate, beveledBorderCol, modelLoc, colorLoc);

  // 2. Faceplate
  mat4 facePlate = translate(mat4(1.0f), pos + vec3(0.0f, 0.0f, 0.015f))
                   * scale(mat4(1.0f), vec3(0.60f, 0.30f, 0.02f));
  drawCube(facePlate, plateCol, modelLoc, colorLoc);

  // 3. 4 Individual Rocker Switches (1, 2, 3, 4)
  bool states[4] = {s1, s2, s3, s4};
  float switchX[4] = {-0.21f, -0.07f, 0.07f, 0.21f};

  for (int i = 0; i < 4; i++) {
    float sx = pos.x + switchX[i];
    bool on = states[i];

    // Recessed switch well
    mat4 well = translate(mat4(1.0f), vec3(sx, pos.y, pos.z + 0.022f))
                * scale(mat4(1.0f), vec3(0.095f, 0.17f, 0.01f));
    drawCube(well, wellCol, modelLoc, colorLoc);

    // Rocker switch toggle (tilts upward when ON, tilts downward when OFF)
    float tiltAngle = on ? -15.0f : 15.0f;
    mat4 rocker = translate(mat4(1.0f), vec3(sx, pos.y, pos.z + 0.030f))
                  * rotate(mat4(1.0f), radians(tiltAngle), vec3(1.0f, 0.0f, 0.0f))
                  * scale(mat4(1.0f), vec3(0.08f, 0.14f, 0.016f));
    drawCube(rocker, on ? switchOnCol : switchOffCol, modelLoc, colorLoc);

    // Small LED status indicator light above the switch (Green when ON, Red when OFF)
    mat4 led = translate(mat4(1.0f), vec3(sx, pos.y + 0.105f, pos.z + 0.026f))
               * scale(mat4(1.0f), vec3(0.028f, 0.028f, 0.012f));
    if (on) {
      setEmissive(true);
      drawCube(led, ledOnCol, modelLoc, colorLoc);
      setEmissive(false);
    } else {
      drawCube(led, ledOffCol, modelLoc, colorLoc);
    }

    // Tactile switch bottom tab
    mat4 tab = translate(mat4(1.0f), vec3(sx, pos.y - 0.105f, pos.z + 0.026f))
               * scale(mat4(1.0f), vec3(0.045f, 0.016f, 0.012f));
    drawCube(tab, labelCol, modelLoc, colorLoc);
  }
}
