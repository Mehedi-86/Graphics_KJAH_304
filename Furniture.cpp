#include "Furniture.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 1. Ceiling Fan: Completely stationary/static at all times.
// Anchored securely to ceiling with canopy, rod, rounded motor hub, and 4
// aerodynamic blades.
void drawCeilingFan(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  // Ceiling Mounting Canopy (spherical cap flush with ceiling at y = 3.50)
  mat4 canopy = translate(mat4(1.0f), pos + vec3(0.0f, 0.46f, 0.0f));
  canopy = scale(canopy, vec3(0.24f, 0.09f, 0.24f));
  drawSphere(canopy, vec3(0.88f, 0.86f, 0.82f), modelLoc, colorLoc);

  // Vertical Connecting Downrod (cylinder)
  mat4 rod = translate(mat4(1.0f), pos + vec3(0.0f, 0.27f, 0.0f));
  rod = scale(rod, vec3(0.038f, 0.38f, 0.038f));
  drawCylinder(rod, vec3(0.80f, 0.78f, 0.73f), modelLoc, colorLoc);

  // --- STATIONARY ASSEMBLY (Static at all times, no rotation animation) ---
  mat4 staticAssembly = translate(mat4(1.0f), pos);

  // Motor Top Collar (cylinder)
  mat4 collar = translate(staticAssembly, vec3(0.0f, 0.06f, 0.0f));
  collar = scale(collar, vec3(0.12f, 0.08f, 0.12f));
  drawCylinder(collar, vec3(0.82f, 0.80f, 0.75f), modelLoc, colorLoc);

  // Center Motor Body: realistic round casing using flattened sphere
  mat4 motorBody = scale(staticAssembly, vec3(0.48f, 0.17f, 0.48f));
  drawSphere(motorBody, vec3(0.92f, 0.90f, 0.84f), modelLoc, colorLoc);

  // Motor Bottom Accent Cap (flattened sphere)
  mat4 botCap = translate(staticAssembly, vec3(0.0f, -0.07f, 0.0f));
  botCap = scale(botCap, vec3(0.22f, 0.05f, 0.22f));
  drawSphere(botCap, vec3(0.76f, 0.74f, 0.68f), modelLoc, colorLoc);

  // 4 Aerodynamic Blades angled in 90-degree increments (Stationary)
  for (int i = 0; i < 4; i++) {
    mat4 bladeBase =
        rotate(staticAssembly, radians(i * 90.0f), vec3(0.0f, 1.0f, 0.0f));

    // Blade metallic mounting arm / bracket
    mat4 arm = translate(bladeBase, vec3(0.32f, 0.01f, 0.0f));
    arm = scale(arm, vec3(0.22f, 0.03f, 0.06f));
    drawCube(arm, vec3(0.62f, 0.60f, 0.55f), modelLoc, colorLoc);

    // Aerodynamic Blade: thin, wide, slightly tilted
    mat4 blade = translate(bladeBase, vec3(0.88f, 0.01f, 0.0f));
    blade = rotate(blade, radians(7.5f), vec3(1.0f, 0.0f, 0.0f));
    blade = scale(blade, vec3(0.94f, 0.02f, 0.25f));
    drawCube(blade, vec3(0.94f, 0.92f, 0.86f), modelLoc, colorLoc);
  }
}

// 2. Almirah: Detailed structure with base, shell, top cornice, 2 distinct door
// panels with slit, and metallic handles
void drawAlmirah(const vec3 &pos, bool isWood, GLuint modelLoc,
                 GLuint colorLoc) {
  vec3 bodyCol = isWood ? vec3(0.38f, 0.20f, 0.09f) : vec3(0.50f, 0.55f, 0.60f);
  vec3 trimCol = isWood ? vec3(0.28f, 0.14f, 0.06f) : vec3(0.40f, 0.44f, 0.48f);
  vec3 panelCol =
      isWood ? vec3(0.44f, 0.24f, 0.11f) : vec3(0.56f, 0.61f, 0.66f);
  vec3 handleCol =
      isWood ? vec3(0.85f, 0.72f, 0.35f) : vec3(0.88f, 0.88f, 0.92f);

  float totalH = 4.5f;
  float width = 1.35f;
  float depth = 1.55f;

  // Recessed Base / Stand at the bottom
  mat4 base =
      translate(mat4(1.0f), pos + vec3(0.0f, -totalH * 0.5f + 0.15f, 0.0f));
  base = scale(base, vec3(width * 0.92f, 0.30f, depth * 0.92f));
  drawCube(base, trimCol, modelLoc, colorLoc);

  // Main Outer Cabinet Shell
  mat4 shell = translate(mat4(1.0f), pos + vec3(0.0f, 0.08f, 0.0f));
  shell = scale(shell, vec3(width, totalH - 0.50f, depth));
  drawCube(shell, bodyCol, modelLoc, colorLoc);

  // Slightly Extruded Top Cornice (Molding ledge)
  mat4 cornice =
      translate(mat4(1.0f), pos + vec3(0.0f, totalH * 0.5f - 0.08f, 0.0f));
  cornice = scale(cornice, vec3(width * 1.08f, 0.16f, depth * 1.08f));
  drawCube(cornice, trimCol, modelLoc, colorLoc);

  // 2 Distinct Front Door Panels with subtle vertical slit
  float doorW = (width * 0.94f - 0.03f) * 0.5f;
  float doorH = totalH - 0.74f;
  float doorD = 0.035f;
  float doorZ = depth * 0.5f + 0.015f;

  float doorX[2] = {-(doorW * 0.5f + 0.015f), (doorW * 0.5f + 0.015f)};
  for (int d = 0; d < 2; d++) {
    // Door face
    mat4 door = translate(mat4(1.0f), pos + vec3(doorX[d], 0.05f, doorZ));
    mat4 doorFace = scale(door, vec3(doorW, doorH, doorD));
    drawCube(doorFace, panelCol, modelLoc, colorLoc);

    // Inset decorative recessed panel frame
    mat4 inset = translate(door, vec3(0.0f, 0.0f, 0.015f));
    inset = scale(inset, vec3(doorW * 0.78f, doorH * 0.82f, 0.02f));
    drawCube(inset, bodyCol * 0.88f, modelLoc, colorLoc);
  }

  // Two Small Metallic Vertical Door Handles
  float handleX[2] = {-0.065f, 0.065f};
  for (int h = 0; h < 2; h++) {
    // Vertical grip bar
    mat4 bar =
        translate(mat4(1.0f), pos + vec3(handleX[h], 0.10f, doorZ + 0.05f));
    bar = scale(bar, vec3(0.025f, 0.35f, 0.025f));
    drawCube(bar, handleCol, modelLoc, colorLoc);

    // Upper & lower standoffs
    mat4 topStandoff =
        translate(mat4(1.0f), pos + vec3(handleX[h], 0.24f, doorZ + 0.025f));
    topStandoff = scale(topStandoff, vec3(0.02f, 0.02f, 0.04f));
    drawCube(topStandoff, handleCol, modelLoc, colorLoc);

    mat4 botStandoff =
        translate(mat4(1.0f), pos + vec3(handleX[h], -0.04f, doorZ + 0.025f));
    botStandoff = scale(botStandoff, vec3(0.02f, 0.02f, 0.04f));
    drawCube(botStandoff, handleCol, modelLoc, colorLoc);
  }
}

// 3. Pillow: Perfectly proportioned, naturally rounded appearance scaling
// sphere. Sits completely within mattress boundaries and snug against the
// interior face of the headboard.
void drawPillow(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  // Soft rounded pillow body (ellipsoid)
  mat4 pillow = translate(mat4(1.0f), pos);
  mat4 body = scale(pillow, vec3(0.62f, 0.10f, 0.28f));
  drawSphere(body, vec3(0.96f, 0.96f, 0.94f), modelLoc, colorLoc);

  // Subtle edge piping / sham border for soft realistic definition
  mat4 border = scale(pillow, vec3(0.64f, 0.025f, 0.29f));
  drawSphere(border, vec3(0.88f, 0.88f, 0.86f), modelLoc, colorLoc);
}

// 4. Enhanced Bed Design:
// 4 corner posts/legs, side rails, recessed mattress, detailed headboard +
// footboard, and realistic layered bedding.
void drawBed(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  vec3 woodCol = vec3(0.38f, 0.20f, 0.08f);
  vec3 woodTrimCol = vec3(0.30f, 0.15f, 0.06f);
  vec3 woodPanelCol = vec3(0.46f, 0.25f, 0.11f);
  vec3 mattressCol = vec3(0.88f, 0.88f, 0.90f);
  vec3 sheetCol = vec3(0.96f, 0.96f, 0.98f);
  vec3 duvetCol = vec3(0.24f, 0.36f, 0.48f);
  vec3 runnerCol = vec3(0.70f, 0.58f, 0.38f);

  // --- 4 CORNER POSTS / LEGS ---
  // Two tall headboard posts
  float postX[2] = {-1.02f, 1.02f};
  for (int i = 0; i < 2; i++) {
    // Headboard post
    mat4 hPost = translate(mat4(1.0f), pos + vec3(postX[i], 0.15f, -1.72f));
    hPost = scale(hPost, vec3(0.12f, 1.70f, 0.12f));
    drawCube(hPost, woodCol, modelLoc, colorLoc);

    // Headboard post finial cap
    mat4 hCap = translate(mat4(1.0f), pos + vec3(postX[i], 1.03f, -1.72f));
    hCap = scale(hCap, vec3(0.14f, 0.06f, 0.14f));
    drawCube(hCap, woodTrimCol, modelLoc, colorLoc);

    // Footboard post
    mat4 fPost = translate(mat4(1.0f), pos + vec3(postX[i], -0.175f, 1.72f));
    fPost = scale(fPost, vec3(0.12f, 1.05f, 0.12f));
    drawCube(fPost, woodCol, modelLoc, colorLoc);

    // Footboard post finial cap
    mat4 fCap = translate(mat4(1.0f), pos + vec3(postX[i], 0.38f, 1.72f));
    fCap = scale(fCap, vec3(0.14f, 0.06f, 0.14f));
    drawCube(fCap, woodTrimCol, modelLoc, colorLoc);
  }

  // --- SIDE RAILS ---
  // Left and right structural side rails connecting posts
  for (int i = 0; i < 2; i++) {
    mat4 rail = translate(mat4(1.0f), pos + vec3(postX[i], -0.32f, 0.0f));
    rail = scale(rail, vec3(0.06f, 0.24f, 3.32f));
    drawCube(rail, woodCol, modelLoc, colorLoc);
  }

  // Mattress base support slats / bottom board
  mat4 slatBase = translate(mat4(1.0f), pos + vec3(0.0f, -0.42f, 0.0f));
  slatBase = scale(slatBase, vec3(1.98f, 0.04f, 3.32f));
  drawCube(slatBase, woodTrimCol, modelLoc, colorLoc);

  // --- DETAILED HEADBOARD ---
  // Headboard back panel (interior face at z = -1.68)
  mat4 headPanel = translate(mat4(1.0f), pos + vec3(0.0f, 0.35f, -1.72f));
  headPanel = scale(headPanel, vec3(1.94f, 1.10f, 0.08f));
  drawCube(headPanel, woodCol, modelLoc, colorLoc);

  // Headboard top crown rail / molding
  mat4 headCrown = translate(mat4(1.0f), pos + vec3(0.0f, 0.93f, -1.72f));
  headCrown = scale(headCrown, vec3(2.06f, 0.06f, 0.12f));
  drawCube(headCrown, woodTrimCol, modelLoc, colorLoc);

  // Two decorative recessed inset panels on interior face of headboard
  mat4 hInset1 = translate(mat4(1.0f), pos + vec3(-0.48f, 0.40f, -1.665f));
  hInset1 = scale(hInset1, vec3(0.70f, 0.65f, 0.02f));
  drawCube(hInset1, woodPanelCol, modelLoc, colorLoc);

  mat4 hInset2 = translate(mat4(1.0f), pos + vec3(0.48f, 0.40f, -1.665f));
  hInset2 = scale(hInset2, vec3(0.70f, 0.65f, 0.02f));
  drawCube(hInset2, woodPanelCol, modelLoc, colorLoc);

  // --- DETAILED FOOTBOARD ---
  // Footboard panel
  mat4 footPanel = translate(mat4(1.0f), pos + vec3(0.0f, -0.07f, 1.72f));
  footPanel = scale(footPanel, vec3(1.94f, 0.50f, 0.08f));
  drawCube(footPanel, woodCol, modelLoc, colorLoc);

  // Footboard top rail
  mat4 footRail = translate(mat4(1.0f), pos + vec3(0.0f, 0.20f, 1.72f));
  footRail = scale(footRail, vec3(2.06f, 0.06f, 0.10f));
  drawCube(footRail, woodTrimCol, modelLoc, colorLoc);

  // Footboard decorative inset panel
  mat4 fInset = translate(mat4(1.0f), pos + vec3(0.0f, -0.07f, 1.705f));
  fInset = scale(fInset, vec3(1.60f, 0.32f, 0.02f));
  drawCube(fInset, woodPanelCol, modelLoc, colorLoc);

  // --- RECESSED MATTRESS ---
  // Mattress body nestled inside rails and headboard/footboard
  mat4 mattress = translate(mat4(1.0f), pos + vec3(0.0f, -0.215f, 0.0f));
  mattress = scale(mattress, vec3(1.94f, 0.35f, 3.34f));
  drawCube(mattress, mattressCol, modelLoc, colorLoc);

  // Fitted sheet at the head region
  mat4 headSheet = translate(mat4(1.0f), pos + vec3(0.0f, -0.035f, -1.05f));
  headSheet = scale(headSheet, vec3(1.92f, 0.015f, 1.22f));
  drawCube(headSheet, sheetCol, modelLoc, colorLoc);

  // Warm duvet/comforter over the lower section
  mat4 duvet = translate(mat4(1.0f), pos + vec3(0.0f, -0.025f, 0.45f));
  duvet = scale(duvet, vec3(1.94f, 0.035f, 2.30f));
  drawCube(duvet, duvetCol, modelLoc, colorLoc);

  // Folded turnover sheet top border
  mat4 turnover = translate(mat4(1.0f), pos + vec3(0.0f, -0.015f, -0.68f));
  turnover = scale(turnover, vec3(1.92f, 0.025f, 0.16f));
  drawCube(turnover, sheetCol, modelLoc, colorLoc);

  // Bed runner accent near foot of bed
  mat4 runner = translate(mat4(1.0f), pos + vec3(0.0f, -0.010f, 1.20f));
  runner = scale(runner, vec3(1.94f, 0.02f, 0.45f));
  drawCube(runner, runnerCol, modelLoc, colorLoc);
}

// 5. Study Table model
void drawStudyTable(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  // Tabletop
  mat4 top = translate(mat4(1.0f), pos);
  top = scale(top, vec3(2.0f, 0.15f, 1.2f));
  drawCube(top, vec3(0.8f, 0.45f, 0.15f), modelLoc, colorLoc);

  // 4 Legs
  float tLegX[2] = {-0.9f, 0.9f};
  float tLegZ[2] = {-0.5f, 0.5f};
  for (int lx = 0; lx < 2; lx++) {
    for (int lz = 0; lz < 2; lz++) {
      mat4 leg = translate(mat4(1.0f), pos + vec3(tLegX[lx], -0.5f, tLegZ[lz]));
      leg = scale(leg, vec3(0.12f, 0.85f, 0.12f));
      drawCube(leg, vec3(0.5f, 0.25f, 0.05f), modelLoc, colorLoc);
    }
  }
}

// 6. Chair model
void drawChair(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  // Metal legs
  float cLegX[2] = {-0.3f, 0.3f};
  float cLegZ[2] = {-0.3f, 0.3f};
  for (int lx = 0; lx < 2; lx++) {
    for (int lz = 0; lz < 2; lz++) {
      mat4 leg =
          translate(mat4(1.0f), pos + vec3(cLegX[lx], -0.45f, cLegZ[lz]));
      leg = scale(leg, vec3(0.08f, 0.8f, 0.08f));
      drawCube(leg, vec3(0.25f, 0.25f, 0.25f), modelLoc, colorLoc);
    }
  }

  // Wooden Seat
  mat4 seat = translate(mat4(1.0f), pos);
  seat = scale(seat, vec3(0.7f, 0.1f, 0.7f));
  drawCube(seat, vec3(0.65f, 0.38f, 0.18f), modelLoc, colorLoc);

  // Backrest uprights
  mat4 up1 = translate(mat4(1.0f), pos + vec3(-0.3f, 0.3f, -0.3f));
  up1 = scale(up1, vec3(0.06f, 0.5f, 0.06f));
  drawCube(up1, vec3(0.25f, 0.25f, 0.25f), modelLoc, colorLoc);

  mat4 up2 = translate(mat4(1.0f), pos + vec3(0.3f, 0.3f, -0.3f));
  up2 = scale(up2, vec3(0.06f, 0.5f, 0.06f));
  drawCube(up2, vec3(0.25f, 0.25f, 0.25f), modelLoc, colorLoc);

  // Horizontal slats
  for (int s = 0; s < 4; s++) {
    mat4 slat =
        translate(mat4(1.0f), pos + vec3(0.0f, 0.15f + s * 0.15f, -0.3f));
    slat = scale(slat, vec3(0.65f, 0.08f, 0.08f));
    drawCube(slat, vec3(0.65f, 0.38f, 0.18f), modelLoc, colorLoc);
  }
}

// 7. Tea Table model
void drawTeaTable(const vec3 &pos, GLuint modelLoc, GLuint colorLoc) {
  // Tabletop
  mat4 top = translate(mat4(1.0f), pos);
  top = scale(top, vec3(1.5f, 0.15f, 1.2f));
  drawCube(top, vec3(0.75f, 0.5f, 0.2f), modelLoc, colorLoc);

  // 4 Legs
  float ttLegX[2] = {-0.6f, 0.6f};
  float ttLegZ[2] = {-0.5f, 0.5f};
  for (int lx = 0; lx < 2; lx++) {
    for (int lz = 0; lz < 2; lz++) {
      mat4 leg =
          translate(mat4(1.0f), pos + vec3(ttLegX[lx], -0.4f, ttLegZ[lz]));
      leg = scale(leg, vec3(0.1f, 0.7f, 0.1f));
      drawCube(leg, vec3(0.4f, 0.25f, 0.1f), modelLoc, colorLoc);
    }
  }
}
