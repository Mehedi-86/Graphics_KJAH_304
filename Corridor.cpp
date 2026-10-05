#include "Corridor.h"
#include "Fixtures.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 4b. University Dormitory Hallway Corridor:
// Fully enclosed architectural hallway outside Door 2 ("ROOM 304").
// Features solid continuous floor, runner stripe (raised to prevent Z-fighting),
// full ceiling, opposite dorm room doors ("ROOM 303", "ROOM 305"), adjacent room doors
// ("ROOM 302", "ROOM 306"), a university hall notice board with announcements,
// wall-mounted fire extinguisher, water dispenser, illuminated exit sign, and fluorescent lighting.
void drawCorridor(GLuint modelLoc, GLuint colorLoc) {
  vec3 wallCol = vec3(0.92f, 0.90f, 0.85f);  // Warm collegiate hall wall
  vec3 floorCol = vec3(0.78f, 0.75f, 0.70f); // Polished terrazzo floor tiles
  vec3 darkBackCol = vec3(0.10f, 0.10f, 0.12f); // Dark interior backing for opposite/adjacent rooms
  vec3 baseboardCol = vec3(0.35f, 0.22f, 0.12f);

  // 1. Solid Structural Foundation Plinth beneath corridor down to ground level (y = -5.85f)
  // Ensures zero open gaps or floating walls when looking from any angle
  mat4 corridorPlinth = translate(mat4(1.0f), vec3(2.0f, -4.20f, -11.50f)) *
                        scale(mat4(1.0f), vec3(25.5f, 3.30f, 5.50f));
  drawCube(corridorPlinth, vec3(0.82f, 0.78f, 0.70f), modelLoc, colorLoc);

  // 2. Corridor Floor (Top surface at y = -2.45f, EXACTLY flush with Room 304 floor)
  mat4 cFloor = translate(mat4(1.0f), vec3(2.0f, -2.50f, -11.50f)) *
                scale(mat4(1.0f), vec3(25.0f, 0.10f, 5.20f));
  drawCube(cFloor, floorCol, modelLoc, colorLoc);

  // 3. Hallway Center Runner Strip (Raised strictly +2.5cm ABOVE floor at y = -2.435f, top at y = -2.425f)
  // Completely eliminates Z-fighting with the floor
  mat4 cRunner = translate(mat4(1.0f), vec3(2.0f, -2.435f, -11.50f)) *
                 scale(mat4(1.0f), vec3(25.0f, 0.02f, 2.20f));
  drawCube(cRunner, vec3(0.48f, 0.30f, 0.18f), modelLoc, colorLoc);

  // 4. Corridor Ceiling (Solid ceiling from x = -10.5f to +14.5f, bottom at y = 3.50f)
  mat4 cCeil = translate(mat4(1.0f), vec3(2.0f, 3.54f, -11.50f)) *
               scale(mat4(1.0f), vec3(25.0f, 0.08f, 5.20f));
  drawCube(cCeil, vec3(0.92f, 0.92f, 0.90f), modelLoc, colorLoc);

  // 5. North Corridor Wall (Opposite Wall at z = -14.0f):
  // Segmented with REAL ARCHITECTURAL DOORWAY CUTOUTS for Room 303 (x = -1.0f) and Room 305 (x = 8.5f)
  // This physically eliminates the wall behind door slabs, completely preventing Z-fighting!

  // Segment 1: West of Door 303 (from x = -10.50f to -1.72f, width 8.78f)
  mat4 nwSeg1 = translate(mat4(1.0f), vec3(-6.11f, 0.50f, -14.0f)) *
                scale(mat4(1.0f), vec3(8.78f, 6.05f, 0.20f));
  drawCube(nwSeg1, wallCol, modelLoc, colorLoc);

  // Header above Door 303 (from x = -1.72f to -0.28f, y = 1.58f to 3.55f, height 1.97f)
  mat4 nwTop1 = translate(mat4(1.0f), vec3(-1.00f, 2.565f, -14.0f)) *
                scale(mat4(1.0f), vec3(1.44f, 1.97f, 0.20f));
  drawCube(nwTop1, wallCol, modelLoc, colorLoc);

  // Segment 2: Between Door 303 and Door 305 (from x = -0.28f to 7.78f, width 8.06f)
  mat4 nwSeg2 = translate(mat4(1.0f), vec3(3.75f, 0.50f, -14.0f)) *
                scale(mat4(1.0f), vec3(8.06f, 6.05f, 0.20f));
  drawCube(nwSeg2, wallCol, modelLoc, colorLoc);

  // Header above Door 305 (from x = 7.78f to 9.22f, y = 1.58f to 3.55f, height 1.97f)
  mat4 nwTop2 = translate(mat4(1.0f), vec3(8.50f, 2.565f, -14.0f)) *
                scale(mat4(1.0f), vec3(1.44f, 1.97f, 0.20f));
  drawCube(nwTop2, wallCol, modelLoc, colorLoc);

  // Segment 3: East of Door 305 (from x = 9.22f to 14.50f, width 5.28f)
  mat4 nwSeg3 = translate(mat4(1.0f), vec3(11.86f, 0.50f, -14.0f)) *
                scale(mat4(1.0f), vec3(5.28f, 6.05f, 0.20f));
  drawCube(nwSeg3, wallCol, modelLoc, colorLoc);

  // Dark interior backing plates recessed behind Door 303 & Door 305
  mat4 d303Back = translate(mat4(1.0f), vec3(-1.0f, 0.50f, -14.12f)) *
                  scale(mat4(1.0f), vec3(1.44f, 4.08f, 0.04f));
  drawCube(d303Back, darkBackCol, modelLoc, colorLoc);

  mat4 d305Back = translate(mat4(1.0f), vec3(8.5f, 0.50f, -14.12f)) *
                  scale(mat4(1.0f), vec3(1.44f, 4.08f, 0.04f));
  drawCube(d305Back, darkBackCol, modelLoc, colorLoc);

  // North Wall Baseboard Skirting (rests at y = -2.45f, height 0.18f, center y = -2.36f, cleanly cut at doors)
  mat4 nwBase1 = translate(mat4(1.0f), vec3(-6.13f, -2.36f, -13.88f)) *
                 scale(mat4(1.0f), vec3(8.74f, 0.18f, 0.04f));
  drawCube(nwBase1, baseboardCol, modelLoc, colorLoc);

  mat4 nwBase2 = translate(mat4(1.0f), vec3(3.75f, -2.36f, -13.88f)) *
                 scale(mat4(1.0f), vec3(7.98f, 0.18f, 0.04f));
  drawCube(nwBase2, baseboardCol, modelLoc, colorLoc);

  mat4 nwBase3 = translate(mat4(1.0f), vec3(11.88f, -2.36f, -13.88f)) *
                 scale(mat4(1.0f), vec3(5.24f, 0.18f, 0.04f));
  drawCube(nwBase3, baseboardCol, modelLoc, colorLoc);

  // 6. West Corridor Wall (Left end at x = -10.50f, spans z from -14.1f to -8.9f)
  mat4 cLeftWall = translate(mat4(1.0f), vec3(-10.50f, 0.5f, -11.50f)) *
                   scale(mat4(1.0f), vec3(0.20f, 6.05f, 5.20f));
  drawCube(cLeftWall, wallCol * 0.98f, modelLoc, colorLoc);

  mat4 wBase = translate(mat4(1.0f), vec3(-10.38f, -2.36f, -11.50f)) *
               scale(mat4(1.0f), vec3(0.04f, 0.18f, 4.80f));
  drawCube(wBase, baseboardCol, modelLoc, colorLoc);

  // 7. East Corridor Wall (Right end at x = +14.50f, spans z from -14.1f to -8.9f)
  mat4 cRightWall = translate(mat4(1.0f), vec3(14.50f, 0.5f, -11.50f)) *
                    scale(mat4(1.0f), vec3(0.20f, 6.05f, 5.20f));
  drawCube(cRightWall, wallCol * 0.98f, modelLoc, colorLoc);

  mat4 eBase = translate(mat4(1.0f), vec3(14.38f, -2.36f, -11.50f)) *
               scale(mat4(1.0f), vec3(0.04f, 0.18f, 4.80f));
  drawCube(eBase, baseboardCol, modelLoc, colorLoc);

  // 8. South Corridor Wall sections (enclosing along z = -9.05f, thickness 0.10f):
  // Segmented with real doorway cutouts for Room 302 (x = -9.25f), Room 304 (x = 5.0f), and Room 306 (x = 11.25f)

  // Segment west of Door 302 (from x = -10.50f to -9.97f, width 0.53f)
  mat4 swSeg1 = translate(mat4(1.0f), vec3(-10.235f, 0.50f, -9.05f)) *
                scale(mat4(1.0f), vec3(0.53f, 6.05f, 0.10f));
  drawCube(swSeg1, wallCol, modelLoc, colorLoc);

  // Header above Door 302 (from x = -9.97f to -8.53f, y = 1.58f to 3.55f, height 1.97f)
  mat4 swTop1 = translate(mat4(1.0f), vec3(-9.25f, 2.565f, -9.05f)) *
                scale(mat4(1.0f), vec3(1.44f, 1.97f, 0.10f));
  drawCube(swTop1, wallCol, modelLoc, colorLoc);

  // Segment between Door 302 and Room 304 (from x = -8.53f to -8.00f, width 0.53f)
  mat4 swSeg2 = translate(mat4(1.0f), vec3(-8.265f, 0.50f, -9.05f)) *
                scale(mat4(1.0f), vec3(0.53f, 6.05f, 0.10f));
  drawCube(swSeg2, wallCol, modelLoc, colorLoc);

  // Dark backing plate behind Door 302
  mat4 d302Back = translate(mat4(1.0f), vec3(-9.25f, 0.50f, -8.98f)) *
                  scale(mat4(1.0f), vec3(1.44f, 4.08f, 0.04f));
  drawCube(d302Back, darkBackCol, modelLoc, colorLoc);

  // Outer finish on Room 304's back wall facing into corridor:
  // Left of Door 304 (from x = -8.0f to 4.24f, width 12.24f)
  mat4 hallBackLeft = translate(mat4(1.0f), vec3(-1.88f, 0.5f, -9.05f)) *
                      scale(mat4(1.0f), vec3(12.24f, 6.05f, 0.10f));
  drawCube(hallBackLeft, wallCol, modelLoc, colorLoc);

  // Right of Door 304 (from x = 5.76f to 8.0f, width 2.24f)
  mat4 hallBackRight = translate(mat4(1.0f), vec3(6.88f, 0.5f, -9.05f)) *
                       scale(mat4(1.0f), vec3(2.24f, 6.05f, 0.10f));
  drawCube(hallBackRight, wallCol, modelLoc, colorLoc);

  // Top header of Door 304 (from x = 4.24f to 5.76f, y = 1.58f to 3.55f, height 1.97f)
  mat4 hallBackTop = translate(mat4(1.0f), vec3(5.0f, 2.565f, -9.05f)) *
                     scale(mat4(1.0f), vec3(1.52f, 1.97f, 0.10f));
  drawCube(hallBackTop, wallCol, modelLoc, colorLoc);

  // Segment between Room 304 and Door 306 (from x = 8.00f to 10.53f, width 2.53f)
  mat4 swSeg3 = translate(mat4(1.0f), vec3(9.265f, 0.50f, -9.05f)) *
                scale(mat4(1.0f), vec3(2.53f, 6.05f, 0.10f));
  drawCube(swSeg3, wallCol, modelLoc, colorLoc);

  // Header above Door 306 (from x = 10.53f to 11.97f, y = 1.58f to 3.55f, height 1.97f)
  mat4 swTop2 = translate(mat4(1.0f), vec3(11.25f, 2.565f, -9.05f)) *
                scale(mat4(1.0f), vec3(1.44f, 1.97f, 0.10f));
  drawCube(swTop2, wallCol, modelLoc, colorLoc);

  // Segment east of Door 306 (from x = 11.97f to 14.50f, width 2.53f)
  mat4 swSeg4 = translate(mat4(1.0f), vec3(13.235f, 0.50f, -9.05f)) *
                scale(mat4(1.0f), vec3(2.53f, 6.05f, 0.10f));
  drawCube(swSeg4, wallCol, modelLoc, colorLoc);

  // Dark backing plate behind Door 306
  mat4 d306Back = translate(mat4(1.0f), vec3(11.25f, 0.50f, -8.98f)) *
                  scale(mat4(1.0f), vec3(1.44f, 4.08f, 0.04f));
  drawCube(d306Back, darkBackCol, modelLoc, colorLoc);

  // South Wall Baseboard Skirting (cleanly cut at doors, resting at y = -2.45f)
  mat4 swBase1 = translate(mat4(1.0f), vec3(-10.255f, -2.36f, -9.12f)) *
                 scale(mat4(1.0f), vec3(0.49f, 0.18f, 0.04f));
  drawCube(swBase1, baseboardCol, modelLoc, colorLoc);

  mat4 swBase2 = translate(mat4(1.0f), vec3(-2.125f, -2.36f, -9.12f)) *
                 scale(mat4(1.0f), vec3(12.73f, 0.18f, 0.04f));
  drawCube(swBase2, baseboardCol, modelLoc, colorLoc);

  mat4 swBase3 = translate(mat4(1.0f), vec3(8.125f, -2.36f, -9.12f)) *
                 scale(mat4(1.0f), vec3(4.73f, 0.18f, 0.04f));
  drawCube(swBase3, baseboardCol, modelLoc, colorLoc);

  mat4 swBase4 = translate(mat4(1.0f), vec3(13.255f, -2.36f, -9.12f)) *
                 scale(mat4(1.0f), vec3(2.49f, 0.18f, 0.04f));
  drawCube(swBase4, baseboardCol, modelLoc, colorLoc);

  // 9. Dorm Doors along the corridor:
  // Opposite doors on North wall: "ROOM 303" and "ROOM 305" (facing corridor in +Z direction)
  drawDoor(vec3(-1.0f, -0.5f, -14.0f), false, 0.0f, "ROOM 303", modelLoc, colorLoc);
  drawDoor(vec3(8.5f, -0.5f, -14.0f), false, 0.0f, "ROOM 305", modelLoc, colorLoc);

  // Adjacent room doors on South wall: "ROOM 302" and "ROOM 306" (facing corridor in -Z direction)
  drawDoor(vec3(-9.25f, -0.5f, -9.05f), true, 0.0f, "ROOM 302", modelLoc, colorLoc);
  drawDoor(vec3(11.25f, -0.5f, -9.05f), true, 0.0f, "ROOM 306", modelLoc, colorLoc);

  // Brass Plaque over Door 304 on corridor side
  mat4 d304Plaque = translate(mat4(1.0f), vec3(5.0f, 1.72f, -9.125f)) *
                    scale(mat4(1.0f), vec3(0.92f, 0.22f, 0.035f));
  drawCube(d304Plaque, vec3(0.18f, 0.14f, 0.10f), modelLoc, colorLoc);

  mat4 d304Face = translate(mat4(1.0f), vec3(5.0f, 1.72f, -9.14f)) *
                  scale(mat4(1.0f), vec3(0.84f, 0.16f, 0.012f));
  drawCube(d304Face, vec3(0.15f, 0.35f, 0.60f), modelLoc, colorLoc);

  // 10. University Hall Notice Board (Directly opposite Door 304 at x = 3.8f)
  mat4 nbFrame = translate(mat4(1.0f), vec3(3.80f, 0.65f, -13.88f)) *
                 scale(mat4(1.0f), vec3(2.40f, 1.40f, 0.04f));
  drawCube(nbFrame, vec3(0.42f, 0.24f, 0.12f), modelLoc, colorLoc);

  mat4 nbCork = translate(mat4(1.0f), vec3(3.80f, 0.65f, -13.86f)) *
                scale(mat4(1.0f), vec3(2.20f, 1.20f, 0.02f));
  drawCube(nbCork, vec3(0.76f, 0.62f, 0.42f), modelLoc, colorLoc);

  mat4 nbHeader = translate(mat4(1.0f), vec3(3.80f, 1.25f, -13.84f)) *
                  scale(mat4(1.0f), vec3(1.60f, 0.16f, 0.02f));
  drawCube(nbHeader, vec3(0.15f, 0.35f, 0.60f), modelLoc, colorLoc);

  // Pinned Notices
  mat4 note1 = translate(mat4(1.0f), vec3(3.15f, 0.65f, -13.84f)) *
               scale(mat4(1.0f), vec3(0.42f, 0.58f, 0.015f));
  drawCube(note1, vec3(0.96f, 0.96f, 0.92f), modelLoc, colorLoc);

  mat4 note2 = translate(mat4(1.0f), vec3(3.80f, 0.70f, -13.84f)) *
               scale(mat4(1.0f), vec3(0.45f, 0.52f, 0.015f));
  drawCube(note2, vec3(0.98f, 0.88f, 0.22f), modelLoc, colorLoc);

  mat4 note3 = translate(mat4(1.0f), vec3(4.45f, 0.65f, -13.84f)) *
               scale(mat4(1.0f), vec3(0.42f, 0.58f, 0.015f));
  drawCube(note3, vec3(0.20f, 0.72f, 0.82f), modelLoc, colorLoc);

  mat4 pin1 = translate(mat4(1.0f), vec3(3.15f, 0.92f, -13.82f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.02f));
  drawSphere(pin1, vec3(0.85f, 0.15f, 0.15f), modelLoc, colorLoc);
  mat4 pin2 = translate(mat4(1.0f), vec3(3.80f, 0.94f, -13.82f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.02f));
  drawSphere(pin2, vec3(0.15f, 0.45f, 0.85f), modelLoc, colorLoc);
  mat4 pin3 = translate(mat4(1.0f), vec3(4.45f, 0.92f, -13.82f)) * scale(mat4(1.0f), vec3(0.025f, 0.025f, 0.02f));
  drawSphere(pin3, vec3(0.15f, 0.75f, 0.25f), modelLoc, colorLoc);

  // 11. Wall-Mounted Fire Extinguisher (at x = 2.0f)
  mat4 feBracket = translate(mat4(1.0f), vec3(2.0f, 0.20f, -13.88f)) *
                   scale(mat4(1.0f), vec3(0.24f, 0.35f, 0.04f));
  drawCube(feBracket, vec3(0.20f, 0.20f, 0.22f), modelLoc, colorLoc);

  mat4 feBody = translate(mat4(1.0f), vec3(2.0f, 0.20f, -13.78f)) *
                scale(mat4(1.0f), vec3(0.12f, 0.55f, 0.12f));
  drawCylinder(feBody, vec3(0.85f, 0.12f, 0.12f), modelLoc, colorLoc);

  mat4 feCap = translate(mat4(1.0f), vec3(2.0f, 0.48f, -13.78f)) *
               scale(mat4(1.0f), vec3(0.115f, 0.08f, 0.115f));
  drawSphere(feCap, vec3(0.85f, 0.12f, 0.12f), modelLoc, colorLoc);

  mat4 feBand = translate(mat4(1.0f), vec3(2.0f, 0.22f, -13.77f)) *
                scale(mat4(1.0f), vec3(0.125f, 0.20f, 0.125f));
  drawCylinder(feBand, vec3(0.95f, 0.85f, 0.20f), modelLoc, colorLoc);

  mat4 feValve = translate(mat4(1.0f), vec3(2.0f, 0.56f, -13.78f)) *
                 scale(mat4(1.0f), vec3(0.04f, 0.08f, 0.04f));
  drawCube(feValve, vec3(0.20f, 0.20f, 0.20f), modelLoc, colorLoc);

  mat4 feHandle = translate(mat4(1.0f), vec3(2.04f, 0.60f, -13.78f)) *
                  scale(mat4(1.0f), vec3(0.14f, 0.02f, 0.03f));
  drawCube(feHandle, vec3(0.85f, 0.12f, 0.12f), modelLoc, colorLoc);

  mat4 feHose = translate(mat4(1.0f), vec3(2.08f, 0.32f, -13.74f)) *
                scale(mat4(1.0f), vec3(0.025f, 0.38f, 0.025f));
  drawCylinder(feHose, vec3(0.12f, 0.12f, 0.12f), modelLoc, colorLoc);

  // 12. Hall Water Cooler / Dispenser (at x = 5.6f)
  mat4 wcCabinet = translate(mat4(1.0f), vec3(5.60f, -1.50f, -13.60f)) *
                   scale(mat4(1.0f), vec3(0.48f, 1.90f, 0.46f));
  drawCube(wcCabinet, vec3(0.90f, 0.90f, 0.92f), modelLoc, colorLoc);

  mat4 wcAlcove = translate(mat4(1.0f), vec3(5.60f, -0.65f, -13.40f)) *
                  scale(mat4(1.0f), vec3(0.36f, 0.32f, 0.08f));
  drawCube(wcAlcove, vec3(0.18f, 0.20f, 0.24f), modelLoc, colorLoc);

  mat4 tapHot = translate(mat4(1.0f), vec3(5.52f, -0.56f, -13.38f)) *
                scale(mat4(1.0f), vec3(0.03f, 0.06f, 0.04f));
  drawCube(tapHot, vec3(0.85f, 0.20f, 0.20f), modelLoc, colorLoc);
  mat4 tapCold = translate(mat4(1.0f), vec3(5.68f, -0.56f, -13.38f)) *
                 scale(mat4(1.0f), vec3(0.03f, 0.06f, 0.04f));
  drawCube(tapCold, vec3(0.20f, 0.50f, 0.90f), modelLoc, colorLoc);

  mat4 wcBottle = translate(mat4(1.0f), vec3(5.60f, -0.15f, -13.60f)) *
                  scale(mat4(1.0f), vec3(0.36f, 0.65f, 0.36f));
  drawCylinder(wcBottle, vec3(0.22f, 0.65f, 0.92f), modelLoc, colorLoc);

  // 13. Emergency Exit / Stairs Sign on West End Wall (x = -10.35f, emissive green)
  mat4 exitSign = translate(mat4(1.0f), vec3(-10.38f, 2.20f, -11.50f)) *
                  scale(mat4(1.0f), vec3(0.03f, 0.32f, 0.70f));
  setEmissive(true);
  drawCube(exitSign, vec3(0.10f, 0.92f, 0.30f), modelLoc, colorLoc);
  setEmissive(false);

  // 14. 3 Overhead Fluorescent Ceiling Light Fixtures with glowing tubes
  float hallLightX[3] = {-4.0f, 3.5f, 10.0f};
  for (int l = 0; l < 3; l++) {
    mat4 fixture = translate(mat4(1.0f), vec3(hallLightX[l], 3.46f, -11.50f)) *
                   scale(mat4(1.0f), vec3(2.40f, 0.06f, 0.30f));
    drawCube(fixture, vec3(0.30f, 0.30f, 0.32f), modelLoc, colorLoc);

    mat4 tube = translate(mat4(1.0f), vec3(hallLightX[l], 3.42f, -11.50f)) *
                scale(mat4(1.0f), vec3(2.20f, 0.04f, 0.16f));
    setEmissive(true);
    drawCube(tube, vec3(0.98f, 0.98f, 0.92f), modelLoc, colorLoc);
    setEmissive(false);
  }
}
