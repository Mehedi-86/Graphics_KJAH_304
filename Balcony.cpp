#include "Balcony.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 4b. Wing Balcony: Architectural standard dorm balcony on hall wings
// Perfectly matches Room 304's master balcony styling:
// Concrete cantilever foundation slab, 2 structural corbels beneath,
// non-slip sandstone patio floor, slate coping edge trim,
// 4 structural iron posts with pyramid caps, rich teak wooden top handrails on all 3 sides,
// bottom and mid iron runner bars, vertical iron baluster spindles on all 3 sides,
// and a wooden planter box on the front handrail with lush green foliage and colorful blooming flowers!
void drawWingBalcony(const vec3 &center, float width, float depth, int dir,
                     GLuint modelLoc, GLuint colorLoc, int flowerSeed) {
  vec3 concreteCol = vec3(0.58f, 0.60f, 0.62f);
  vec3 tileFloorCol = vec3(0.74f, 0.70f, 0.65f);
  vec3 slateTrimCol = vec3(0.32f, 0.33f, 0.36f);
  vec3 ironRailingCol = vec3(0.22f, 0.23f, 0.25f);
  vec3 teakHandrailCol = vec3(0.44f, 0.24f, 0.10f);

  float rotY = 0.0f;
  if (dir == 0) rotY = 0.0f;        // Near wing (faces +Z)
  else if (dir == 1) rotY = 180.0f; // Reverse wing (faces -Z)
  else if (dir == 2) rotY = 90.0f;  // Left wing (faces +X)
  else if (dir == 3) rotY = -90.0f; // Right wing (faces -X)

  mat4 root = translate(mat4(1.0f), center);
  root = rotate(root, radians(rotY), vec3(0.0f, 1.0f, 0.0f));

  float halfW = width * 0.5f;
  float halfD = depth * 0.5f;

  // 1. Structural Cantilever Corbels underneath the slab
  float corbelX[2] = {-halfW * 0.60f, halfW * 0.60f};
  for (int c = 0; c < 2; c++) {
    mat4 corbel = root * translate(mat4(1.0f), vec3(corbelX[c], -0.42f, -0.10f)) *
                  scale(mat4(1.0f), vec3(0.12f, 0.24f, depth * 0.70f));
    drawCube(corbel, concreteCol * 0.90f, modelLoc, colorLoc);
  }

  // 2. Concrete Cantilever Foundation Slab
  mat4 cSlab = root * translate(mat4(1.0f), vec3(0.0f, -0.26f, 0.0f)) *
               scale(mat4(1.0f), vec3(width + 0.04f, 0.12f, depth + 0.04f));
  drawCube(cSlab, concreteCol, modelLoc, colorLoc);

  // 3. Sandstone Patio Floor Surface
  mat4 bFloor = root * translate(mat4(1.0f), vec3(0.0f, -0.19f, 0.0f)) *
                scale(mat4(1.0f), vec3(width - 0.04f, 0.02f, depth - 0.04f));
  drawCube(bFloor, tileFloorCol, modelLoc, colorLoc);

  // 4. Perimeter Slate Edge Trim (front)
  mat4 fTrim = root * translate(mat4(1.0f), vec3(0.0f, -0.18f, halfD)) *
               scale(mat4(1.0f), vec3(width + 0.04f, 0.04f, 0.06f));
  drawCube(fTrim, slateTrimCol, modelLoc, colorLoc);

  // 5. Two Front Corner Structural Iron Posts with Pyramid Caps
  float pX[2] = {-halfW, halfW};
  for (int p = 0; p < 2; p++) {
    mat4 post = root * translate(mat4(1.0f), vec3(pX[p], 0.16f, halfD)) *
                scale(mat4(1.0f), vec3(0.06f, 0.72f, 0.06f));
    drawCube(post, ironRailingCol, modelLoc, colorLoc);

    mat4 pCap = root * translate(mat4(1.0f), vec3(pX[p], 0.54f, halfD)) *
                scale(mat4(1.0f), vec3(0.08f, 0.04f, 0.08f));
    drawCube(pCap, ironRailingCol * 1.2f, modelLoc, colorLoc);
  }

  // 6. Teak Wooden Top Handrails (Front, Left, Right)
  mat4 fHandrail = root * translate(mat4(1.0f), vec3(0.0f, 0.51f, halfD)) *
                   scale(mat4(1.0f), vec3(width, 0.05f, 0.08f));
  drawCube(fHandrail, teakHandrailCol, modelLoc, colorLoc);

  mat4 lHandrail = root * translate(mat4(1.0f), vec3(-halfW, 0.51f, 0.0f)) *
                   scale(mat4(1.0f), vec3(0.08f, 0.05f, depth));
  drawCube(lHandrail, teakHandrailCol, modelLoc, colorLoc);

  mat4 rHandrail = root * translate(mat4(1.0f), vec3(halfW, 0.51f, 0.0f)) *
                   scale(mat4(1.0f), vec3(0.08f, 0.05f, depth));
  drawCube(rHandrail, teakHandrailCol, modelLoc, colorLoc);

  // 7. Protective Bottom Runner Rails (Front, Left, Right)
  mat4 fBotRail = root * translate(mat4(1.0f), vec3(0.0f, -0.14f, halfD)) *
                  scale(mat4(1.0f), vec3(width, 0.025f, 0.035f));
  drawCube(fBotRail, ironRailingCol, modelLoc, colorLoc);

  mat4 lBotRail = root * translate(mat4(1.0f), vec3(-halfW, -0.14f, 0.0f)) *
                  scale(mat4(1.0f), vec3(0.035f, 0.025f, depth));
  drawCube(lBotRail, ironRailingCol, modelLoc, colorLoc);

  mat4 rBotRail = root * translate(mat4(1.0f), vec3(halfW, -0.14f, 0.0f)) *
                  scale(mat4(1.0f), vec3(0.035f, 0.025f, depth));
  drawCube(rBotRail, ironRailingCol, modelLoc, colorLoc);

  // 8. Vertical Iron Balusters / Spindles (Clean, performant distribution)
  for (int i = 0; i < 3; i++) {
    float bx = -halfW * 0.60f + i * (halfW * 1.20f / 2.0f);
    mat4 bal = root * translate(mat4(1.0f), vec3(bx, 0.18f, halfD)) *
               scale(mat4(1.0f), vec3(0.024f, 0.62f, 0.024f));
    drawCube(bal, ironRailingCol, modelLoc, colorLoc);
  }

  // Side baluster (1 on left, 1 on right)
  mat4 lBal = root * translate(mat4(1.0f), vec3(-halfW, 0.18f, 0.0f)) *
              scale(mat4(1.0f), vec3(0.024f, 0.62f, 0.024f));
  drawCube(lBal, ironRailingCol, modelLoc, colorLoc);

  mat4 rBal = root * translate(mat4(1.0f), vec3(halfW, 0.18f, 0.0f)) *
              scale(mat4(1.0f), vec3(0.024f, 0.62f, 0.024f));
  drawCube(rBal, ironRailingCol, modelLoc, colorLoc);

  // 9. Flower Planter Box Mounted on Front Handrail with Blooming Flowers
  mat4 pBox = root * translate(mat4(1.0f), vec3(0.0f, 0.42f, halfD + 0.05f)) *
              scale(mat4(1.0f), vec3(width * 0.65f, 0.14f, 0.14f));
  drawCube(pBox, vec3(0.38f, 0.20f, 0.10f), modelLoc, colorLoc);

  mat4 pSoil = root * translate(mat4(1.0f), vec3(0.0f, 0.48f, halfD + 0.05f)) *
               scale(mat4(1.0f), vec3(width * 0.60f, 0.03f, 0.11f));
  drawCube(pSoil, vec3(0.18f, 0.12f, 0.08f), modelLoc, colorLoc);

  // Overhanging green foliage clusters & colorful flowers (lightweight cubes)
  vec3 flowerColors[3] = {
      vec3(0.95f, 0.20f, 0.25f), // Crimson red
      vec3(1.00f, 0.85f, 0.15f), // Golden yellow
      vec3(0.75f, 0.25f, 0.85f)  // Purple
  };
  for (int f = -1; f <= 1; f++) {
    mat4 leaf = root * translate(mat4(1.0f), vec3(f * 0.24f, 0.51f, halfD + 0.055f)) *
                scale(mat4(1.0f), vec3(0.18f, 0.09f, 0.14f));
    drawCube(leaf, vec3(0.18f, 0.54f, 0.22f), modelLoc, colorLoc);

    vec3 fCol = flowerColors[(abs(f + flowerSeed)) % 3];
    mat4 flower = root * translate(mat4(1.0f), vec3(f * 0.24f, 0.565f, halfD + 0.06f)) *
                  scale(mat4(1.0f), vec3(0.085f, 0.065f, 0.085f));
    drawCube(flower, fCol, modelLoc, colorLoc);
  }
}

// 4d. Balcony Wooden Door: Simple, small, and narrow collegiate solid wooden door with outer casing,
// threshold sill, recessed wooden panels, and polished brass doorknob (NO GLASS AT ALL).
void drawBalconyDoor(const vec3 &wallPos, float width, float height, int dir,
                     GLuint modelLoc, GLuint colorLoc) {
  // Orientation: local +Z points outward from wall onto balcony
  float rotY = 0.0f;
  if (dir == 0) rotY = 0.0f;        // Near wings (faces +Z)
  else if (dir == 1) rotY = 180.0f; // Reverse wing (faces -Z)
  else if (dir == 2) rotY = 90.0f;  // Left wing (faces +X)
  else if (dir == 3) rotY = -90.0f; // Right wing (faces -X)

  mat4 root = translate(mat4(1.0f), wallPos) * rotate(mat4(1.0f), radians(rotY), vec3(0.0f, 1.0f, 0.0f));

  // Color palette matching Room 304's architectural wooden doors
  vec3 frameCol = vec3(0.88f, 0.88f, 0.85f);       // Off-white outer casing frame
  vec3 sillCol = vec3(0.32f, 0.33f, 0.35f);        // Dark stone threshold sill
  vec3 doorCol = vec3(0.44f, 0.26f, 0.12f);        // Rich warm natural wooden slab
  vec3 panelCol = vec3(0.36f, 0.19f, 0.08f);       // Recessed wooden panels
  vec3 knobCol = vec3(0.90f, 0.80f, 0.38f);        // Polished brass doorknob

  float halfW = width * 0.5f;
  float doorBottomY = -0.19f;              // Aligns flush with balcony patio floor
  float doorTopY = doorBottomY + height;    // Door header height
  float doorCenterY = (doorBottomY + doorTopY) * 0.5f;

  // 1. Heavy Stone / Slate Threshold Sill at Base
  mat4 sill = root * translate(mat4(1.0f), vec3(0.0f, doorBottomY + 0.02f, 0.035f)) *
              scale(mat4(1.0f), vec3(width + 0.06f, 0.05f, 0.12f));
  drawCube(sill, sillCol, modelLoc, colorLoc);

  // 2. Outer Casing Frame (Left jamb, Right jamb, Top lintel)
  mat4 lJamb = root * translate(mat4(1.0f), vec3(-halfW + 0.04f, doorCenterY, 0.02f)) *
               scale(mat4(1.0f), vec3(0.08f, height, 0.08f));
  drawCube(lJamb, frameCol, modelLoc, colorLoc);

  mat4 rJamb = root * translate(mat4(1.0f), vec3(halfW - 0.04f, doorCenterY, 0.02f)) *
               scale(mat4(1.0f), vec3(0.08f, height, 0.08f));
  drawCube(rJamb, frameCol, modelLoc, colorLoc);

  mat4 topLintel = root * translate(mat4(1.0f), vec3(0.0f, doorTopY - 0.035f, 0.02f)) *
                   scale(mat4(1.0f), vec3(width + 0.06f, 0.07f, 0.08f));
  drawCube(topLintel, frameCol, modelLoc, colorLoc);

  // 3. Simple Narrow Solid Wooden Door Slab (Natural wood, NO GLASS)
  float slabW = width - 0.14f;
  float slabH = height - 0.07f;
  mat4 slab = root * translate(mat4(1.0f), vec3(0.0f, doorCenterY - 0.015f, 0.022f)) *
              scale(mat4(1.0f), vec3(slabW, slabH, 0.055f));
  drawCube(slab, doorCol, modelLoc, colorLoc);

  // 4. Two Recessed Wooden Panels on the Narrow Door Leaf (Upper & Lower)
  float panW = slabW * 0.72f;
  // Upper panel
  mat4 uPanel = root * translate(mat4(1.0f), vec3(0.0f, doorBottomY + slabH * 0.72f, 0.035f)) *
                scale(mat4(1.0f), vec3(panW, slabH * 0.38f, 0.020f));
  drawCube(uPanel, panelCol, modelLoc, colorLoc);

  // Lower panel
  mat4 dPanel = root * translate(mat4(1.0f), vec3(0.0f, doorBottomY + slabH * 0.28f, 0.035f)) *
                scale(mat4(1.0f), vec3(panW, slabH * 0.36f, 0.020f));
  drawCube(dPanel, panelCol, modelLoc, colorLoc);

  // 5. Polished Brass Doorknob with Rosette Escutcheon Plate
  float knobX = slabW * 0.34f;
  float knobY = doorBottomY + slabH * 0.48f;
  mat4 rosette = root * translate(mat4(1.0f), vec3(knobX, knobY, 0.048f)) *
                 scale(mat4(1.0f), vec3(0.06f, 0.06f, 0.012f));
  drawCube(rosette, knobCol, modelLoc, colorLoc);

  mat4 knob = root * translate(mat4(1.0f), vec3(knobX, knobY, 0.065f)) *
              scale(mat4(1.0f), vec3(0.04f, 0.04f, 0.04f));
  drawCube(knob, knobCol * 1.05f, modelLoc, colorLoc);
}

// 4. Balcony:
// Spacious outdoor terrace featuring decorative cantilever base, non-slip tiled floor,
// modern protective iron & teak railings, comfortable patio furniture, potted greenery,
// exterior coach lighting, and an expansive outdoor scenic vista.
void drawBalcony(GLuint modelLoc, GLuint colorLoc) {
  vec3 concreteCol = vec3(0.58f, 0.60f, 0.62f);
  vec3 tileFloorCol = vec3(0.74f, 0.70f, 0.65f);
  vec3 slateTrimCol = vec3(0.32f, 0.33f, 0.36f);
  vec3 ironRailingCol = vec3(0.22f, 0.23f, 0.25f);
  vec3 teakHandrailCol = vec3(0.44f, 0.24f, 0.10f);
  vec3 foliageDarkCol = vec3(0.18f, 0.48f, 0.22f);
  vec3 foliageLightCol = vec3(0.28f, 0.64f, 0.30f);
  vec3 terracottaCol = vec3(0.72f, 0.38f, 0.22f);

  // --- 1. CANTILEVER CONCRETE FOUNDATION SLAB & CORBELS ---
  // Thick cantilever support slab beneath the terrace
  mat4 cSlab = translate(mat4(1.0f), vec3(-5.0f, -2.62f, 11.20f));
  cSlab = scale(cSlab, vec3(5.24f, 0.24f, 4.60f));
  drawCube(cSlab, concreteCol, modelLoc, colorLoc);

  // 2 Structural cantilever support corbels under the balcony
  float corbelX[2] = {-6.8f, -3.2f};
  for (int c = 0; c < 2; c++) {
    mat4 corbel = translate(mat4(1.0f), vec3(corbelX[c], -2.90f, 9.80f));
    corbel = scale(corbel, vec3(0.24f, 0.38f, 1.60f));
    drawCube(corbel, concreteCol * 0.90f, modelLoc, colorLoc);
  }

  // --- 2. OUTDOOR TILED FLOOR & SLATE COPING ---
  // Main sandstone balcony floor surface
  mat4 bFloor = translate(mat4(1.0f), vec3(-5.0f, -2.48f, 11.20f));
  bFloor = scale(bFloor, vec3(5.16f, 0.04f, 4.52f));
  drawCube(bFloor, tileFloorCol, modelLoc, colorLoc);

  // Front slate coping trim edge (z = 13.46f)
  mat4 fTrim = translate(mat4(1.0f), vec3(-5.0f, -2.46f, 13.46f));
  fTrim = scale(fTrim, vec3(5.20f, 0.06f, 0.16f));
  drawCube(fTrim, slateTrimCol, modelLoc, colorLoc);

  // Left slate coping trim edge (x = -7.50f)
  mat4 lTrim = translate(mat4(1.0f), vec3(-7.50f, -2.46f, 11.20f));
  lTrim = scale(lTrim, vec3(0.16f, 0.06f, 4.52f));
  drawCube(lTrim, slateTrimCol, modelLoc, colorLoc);

  // Right slate coping trim edge (x = -2.50f)
  mat4 rTrim = translate(mat4(1.0f), vec3(-2.50f, -2.46f, 11.20f));
  rTrim = scale(rTrim, vec3(0.16f, 0.06f, 4.52f));
  drawCube(rTrim, slateTrimCol, modelLoc, colorLoc);

  // Decorative patio tile mosaic diamond inlay in floor center
  mat4 inlay = translate(mat4(1.0f), vec3(-5.0f, -2.45f, 11.20f));
  inlay = rotate(inlay, radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
  inlay = scale(inlay, vec3(1.20f, 0.02f, 1.20f));
  drawCube(inlay, slateTrimCol * 1.15f, modelLoc, colorLoc);

  // --- 3. PERIMETER SAFETY RAILINGS ---
  // 4 Corner structural posts
  float pX[4] = {-7.50f, -7.50f, -2.50f, -2.50f};
  float pZ[4] = {9.00f, 13.46f, 13.46f, 9.00f};
  for (int p = 0; p < 4; p++) {
    // Post column
    mat4 post = translate(mat4(1.0f), vec3(pX[p], -1.85f, pZ[p]));
    post = scale(post, vec3(0.12f, 1.26f, 0.12f));
    drawCube(post, ironRailingCol, modelLoc, colorLoc);

    // Decorative pyramid cap
    mat4 pCap = translate(mat4(1.0f), vec3(pX[p], -1.20f, pZ[p]));
    pCap = scale(pCap, vec3(0.16f, 0.06f, 0.16f));
    drawCube(pCap, ironRailingCol * 1.2f, modelLoc, colorLoc);
  }

  // Front Railing (z = 13.46f, from x = -7.50f to -2.50f)
  // Top teak handrail
  mat4 fHandrail = translate(mat4(1.0f), vec3(-5.0f, -1.24f, 13.46f));
  fHandrail = scale(fHandrail, vec3(4.90f, 0.08f, 0.14f));
  drawCube(fHandrail, teakHandrailCol, modelLoc, colorLoc);

  // Lower runner bar
  mat4 fBotRail = translate(mat4(1.0f), vec3(-5.0f, -2.40f, 13.46f));
  fBotRail = scale(fBotRail, vec3(4.90f, 0.04f, 0.06f));
  drawCube(fBotRail, ironRailingCol, modelLoc, colorLoc);

  // Mid decorative rail
  mat4 fMidRail = translate(mat4(1.0f), vec3(-5.0f, -1.85f, 13.46f));
  fMidRail = scale(fMidRail, vec3(4.90f, 0.03f, 0.04f));
  drawCube(fMidRail, ironRailingCol, modelLoc, colorLoc);

  // 14 Vertical balusters along front
  for (int i = 0; i < 14; i++) {
    float bx = -7.25f + i * 0.35f;
    mat4 bal = translate(mat4(1.0f), vec3(bx, -1.84f, 13.46f));
    bal = scale(bal, vec3(0.035f, 1.14f, 0.035f));
    drawCube(bal, ironRailingCol, modelLoc, colorLoc);
  }

  // Left Railing (x = -7.50f, from z = 9.00f to 13.46f)
  // Top teak handrail
  mat4 lHandrail = translate(mat4(1.0f), vec3(-7.50f, -1.24f, 11.23f));
  lHandrail = scale(lHandrail, vec3(0.14f, 0.08f, 4.36f));
  drawCube(lHandrail, teakHandrailCol, modelLoc, colorLoc);

  mat4 lBotRail = translate(mat4(1.0f), vec3(-7.50f, -2.40f, 11.23f));
  lBotRail = scale(lBotRail, vec3(0.06f, 0.04f, 4.36f));
  drawCube(lBotRail, ironRailingCol, modelLoc, colorLoc);

  mat4 lMidRail = translate(mat4(1.0f), vec3(-7.50f, -1.85f, 11.23f));
  lMidRail = scale(lMidRail, vec3(0.04f, 0.03f, 4.36f));
  drawCube(lMidRail, ironRailingCol, modelLoc, colorLoc);

  for (int i = 0; i < 12; i++) {
    float bz = 9.25f + i * 0.35f;
    mat4 bal = translate(mat4(1.0f), vec3(-7.50f, -1.84f, bz));
    bal = scale(bal, vec3(0.035f, 1.14f, 0.035f));
    drawCube(bal, ironRailingCol, modelLoc, colorLoc);
  }

  // Right Railing (x = -2.50f, from z = 9.00f to 13.46f)
  mat4 rHandrail = translate(mat4(1.0f), vec3(-2.50f, -1.24f, 11.23f));
  rHandrail = scale(rHandrail, vec3(0.14f, 0.08f, 4.36f));
  drawCube(rHandrail, teakHandrailCol, modelLoc, colorLoc);

  mat4 rBotRail = translate(mat4(1.0f), vec3(-2.50f, -2.40f, 11.23f));
  rBotRail = scale(rBotRail, vec3(0.06f, 0.04f, 4.36f));
  drawCube(rBotRail, ironRailingCol, modelLoc, colorLoc);

  mat4 rMidRail = translate(mat4(1.0f), vec3(-2.50f, -1.85f, 11.23f));
  rMidRail = scale(rMidRail, vec3(0.04f, 0.03f, 4.36f));
  drawCube(rMidRail, ironRailingCol, modelLoc, colorLoc);

  for (int i = 0; i < 12; i++) {
    float bz = 9.25f + i * 0.35f;
    mat4 bal = translate(mat4(1.0f), vec3(-2.50f, -1.84f, bz));
    bal = scale(bal, vec3(0.035f, 1.14f, 0.035f));
    drawCube(bal, ironRailingCol, modelLoc, colorLoc);
  }

  // --- 4. PATIO FURNITURE ---
  // Round Outdoor Glass Bistro Table
  mat4 tblBase = translate(mat4(1.0f), vec3(-3.5f, -2.45f, 11.2f));
  tblBase = scale(tblBase, vec3(0.35f, 0.02f, 0.35f));
  drawCylinder(tblBase, ironRailingCol, modelLoc, colorLoc);

  mat4 tblStem = translate(mat4(1.0f), vec3(-3.5f, -2.14f, 11.2f));
  tblStem = scale(tblStem, vec3(0.035f, 0.62f, 0.035f));
  drawCylinder(tblStem, ironRailingCol, modelLoc, colorLoc);

  mat4 tblTop = translate(mat4(1.0f), vec3(-3.5f, -1.82f, 11.2f));
  tblTop = scale(tblTop, vec3(0.55f, 0.025f, 0.55f));
  drawCylinder(tblTop, vec3(0.40f, 0.68f, 0.78f), modelLoc, colorLoc);

  // Cold refreshing iced drink on table
  mat4 bev = translate(mat4(1.0f), vec3(-3.45f, -1.74f, 11.2f));
  bev = scale(bev, vec3(0.06f, 0.14f, 0.06f));
  drawCylinder(bev, vec3(0.95f, 0.55f, 0.20f), modelLoc, colorLoc);

  mat4 straw = translate(mat4(1.0f), vec3(-3.43f, -1.65f, 11.2f));
  straw = rotate(straw, radians(15.0f), vec3(0.0f, 0.0f, 1.0f));
  straw = scale(straw, vec3(0.012f, 0.16f, 0.012f));
  drawCylinder(straw, vec3(0.98f, 0.98f, 0.98f), modelLoc, colorLoc);

  // Outdoor Woven Armchair
  // Seat cushion
  mat4 chairSeat = translate(mat4(1.0f), vec3(-3.5f, -2.04f, 12.2f));
  chairSeat = scale(chairSeat, vec3(0.58f, 0.08f, 0.55f));
  drawCube(chairSeat, vec3(0.36f, 0.54f, 0.42f), modelLoc, colorLoc);

  // 4 Legs
  float cLegX[2] = {-0.24f, 0.24f};
  float cLegZ[2] = {-0.22f, 0.22f};
  for (int lx = 0; lx < 2; lx++) {
    for (int lz = 0; lz < 2; lz++) {
      mat4 leg = translate(mat4(1.0f), vec3(-3.5f + cLegX[lx], -2.26f, 12.2f + cLegZ[lz]));
      leg = scale(leg, vec3(0.04f, 0.42f, 0.04f));
      drawCube(leg, ironRailingCol, modelLoc, colorLoc);
    }
  }
  // Backrest
  mat4 chairBack = translate(mat4(1.0f), vec3(-3.5f, -1.72f, 12.44f));
  chairBack = scale(chairBack, vec3(0.58f, 0.56f, 0.06f));
  drawCube(chairBack, vec3(0.42f, 0.30f, 0.20f), modelLoc, colorLoc);

  // --- 5. LUSH BALCONY POTTED PLANTS ---
  // Large Terracotta Flowerpot (front corner at x = -6.9f, z = 12.8f)
  mat4 pot1 = translate(mat4(1.0f), vec3(-6.9f, -2.22f, 12.8f));
  pot1 = scale(pot1, vec3(0.32f, 0.48f, 0.32f));
  drawCylinder(pot1, terracottaCol, modelLoc, colorLoc);

  mat4 rim1 = translate(mat4(1.0f), vec3(-6.9f, -1.97f, 12.8f));
  rim1 = scale(rim1, vec3(0.35f, 0.06f, 0.35f));
  drawCylinder(rim1, terracottaCol * 1.08f, modelLoc, colorLoc);

  // Bushy layered plant foliage
  mat4 fBush1 = translate(mat4(1.0f), vec3(-6.9f, -1.82f, 12.8f));
  fBush1 = scale(fBush1, vec3(0.38f, 0.28f, 0.38f));
  drawSphere(fBush1, foliageDarkCol, modelLoc, colorLoc);

  mat4 fBush2 = translate(mat4(1.0f), vec3(-6.95f, -1.68f, 12.78f));
  fBush2 = scale(fBush2, vec3(0.30f, 0.22f, 0.30f));
  drawSphere(fBush2, foliageLightCol, modelLoc, colorLoc);

  mat4 fBush3 = translate(mat4(1.0f), vec3(-6.82f, -1.74f, 12.88f));
  fBush3 = scale(fBush3, vec3(0.24f, 0.18f, 0.24f));
  drawSphere(fBush3, foliageDarkCol * 1.1f, modelLoc, colorLoc);

  // Second Glazed Ceramic Pot near doorway (x = -2.9f, z = 9.8f)
  mat4 pot2 = translate(mat4(1.0f), vec3(-2.9f, -2.24f, 9.8f));
  pot2 = scale(pot2, vec3(0.26f, 0.42f, 0.26f));
  drawCylinder(pot2, vec3(0.20f, 0.50f, 0.54f), modelLoc, colorLoc);

  mat4 f2Bush = translate(mat4(1.0f), vec3(-2.9f, -1.92f, 9.8f));
  f2Bush = scale(f2Bush, vec3(0.32f, 0.26f, 0.32f));
  drawSphere(f2Bush, foliageLightCol, modelLoc, colorLoc);

  mat4 flower1 = translate(mat4(1.0f), vec3(-2.88f, -1.78f, 9.82f));
  flower1 = scale(flower1, vec3(0.08f, 0.08f, 0.08f));
  drawSphere(flower1, vec3(0.95f, 0.28f, 0.42f), modelLoc, colorLoc);

  // --- 6. EXTERIOR WALL COACH SCONCE LIGHT ---
  mat4 coachMnt = translate(mat4(1.0f), vec3(-3.85f, -0.20f, 9.12f));
  coachMnt = scale(coachMnt, vec3(0.10f, 0.24f, 0.04f));
  drawCube(coachMnt, ironRailingCol, modelLoc, colorLoc);

  mat4 coachArm = translate(mat4(1.0f), vec3(-3.85f, -0.15f, 9.22f));
  coachArm = scale(coachArm, vec3(0.03f, 0.03f, 0.18f));
  drawCube(coachArm, ironRailingCol, modelLoc, colorLoc);

  mat4 lanternGlass = translate(mat4(1.0f), vec3(-3.85f, -0.28f, 9.30f));
  lanternGlass = scale(lanternGlass, vec3(0.12f, 0.22f, 0.12f));
  drawCube(lanternGlass, vec3(0.95f, 0.88f, 0.65f), modelLoc, colorLoc);

  mat4 lanternBulb = translate(mat4(1.0f), vec3(-3.85f, -0.28f, 9.30f));
  lanternBulb = scale(lanternBulb, vec3(0.06f, 0.06f, 0.06f));
  setEmissive(true);
  drawSphere(lanternBulb, vec3(1.0f, 0.95f, 0.75f), modelLoc, colorLoc);
  setEmissive(false);

  // --- 7. BALCONY RAILING PLANTER BOXES WITH VIBRANT FLOWERS ---
  float boxX[2] = {-6.10f, -3.90f};
  for (int b = 0; b < 2; b++) {
    // Planter box wooden trough mounted on handrail
    mat4 pBox = translate(mat4(1.0f), vec3(boxX[b], -1.38f, 13.52f))
                * scale(mat4(1.0f), vec3(1.10f, 0.22f, 0.22f));
    drawCube(pBox, vec3(0.38f, 0.20f, 0.10f), modelLoc, colorLoc);

    // Soil
    mat4 pSoil = translate(mat4(1.0f), vec3(boxX[b], -1.28f, 13.52f))
                 * scale(mat4(1.0f), vec3(1.04f, 0.05f, 0.18f));
    drawCube(pSoil, vec3(0.18f, 0.12f, 0.08f), modelLoc, colorLoc);

    // Overhanging green foliage clusters & colorful flowers (using fast cube primitives)
    for (int f = -2; f <= 2; f++) {
      mat4 leaf = translate(mat4(1.0f), vec3(boxX[b] + f * 0.20f, -1.22f, 13.54f))
                  * scale(mat4(1.0f), vec3(0.18f, 0.14f, 0.18f));
      drawCube(leaf, vec3(0.20f, 0.58f, 0.24f), modelLoc, colorLoc);

      vec3 fCol = (f % 2 == 0) ? vec3(0.95f, 0.20f, 0.25f)
                               : ((f == 1) ? vec3(1.0f, 0.85f, 0.15f) : vec3(0.75f, 0.25f, 0.85f));
      mat4 flower = translate(mat4(1.0f), vec3(boxX[b] + f * 0.20f, -1.14f, 13.56f))
                    * scale(mat4(1.0f), vec3(0.08f, 0.08f, 0.08f));
      drawCube(flower, fCol, modelLoc, colorLoc);
    }
  }

  // --- 8. CENTRAL PLAYGROUND SPORTS FIELD (COURTYARD BELOW) ---
  // Ground base terrain below the 3rd floor at y = -5.85f
  mat4 fieldGround = translate(mat4(1.0f), vec3(0.0f, -5.85f, 33.0f))
                     * scale(mat4(1.0f), vec3(48.0f, 0.12f, 44.0f));
  drawCube(fieldGround, vec3(0.24f, 0.50f, 0.22f), modelLoc, colorLoc);

  // Red Athletic Running Track encircling the sports field
  mat4 track = translate(mat4(1.0f), vec3(0.0f, -5.80f, 33.0f))
               * scale(mat4(1.0f), vec3(41.0f, 0.05f, 37.0f));
  drawCube(track, vec3(0.70f, 0.32f, 0.20f), modelLoc, colorLoc);

  // White inner track border
  mat4 trackInnerBorder = translate(mat4(1.0f), vec3(0.0f, -5.77f, 33.0f))
                          * scale(mat4(1.0f), vec3(35.2f, 0.05f, 31.2f));
  drawCube(trackInnerBorder, vec3(0.95f, 0.95f, 0.96f), modelLoc, colorLoc);

  // Inner Grass Playing Pitch with alternating mown grass stripes
  mat4 pitchBase = translate(mat4(1.0f), vec3(0.0f, -5.75f, 33.0f))
                   * scale(mat4(1.0f), vec3(34.8f, 0.06f, 30.8f));
  drawCube(pitchBase, vec3(0.22f, 0.56f, 0.20f), modelLoc, colorLoc);

  // 6 Alternating mown grass turf stripes across the pitch
  for (int s = 0; s < 6; s++) {
    float sz = 19.5f + s * 4.6f;
    vec3 stripeCol = (s % 2 == 0) ? vec3(0.26f, 0.62f, 0.24f) : vec3(0.20f, 0.52f, 0.18f);
    mat4 stripe = translate(mat4(1.0f), vec3(0.0f, -5.73f, sz))
                  * scale(mat4(1.0f), vec3(34.4f, 0.04f, 4.4f));
    drawCube(stripe, stripeCol, modelLoc, colorLoc);
  }

  // White Regulation Touchline Boundaries
  mat4 touchlines = translate(mat4(1.0f), vec3(0.0f, -5.71f, 33.0f))
                    * scale(mat4(1.0f), vec3(31.0f, 0.03f, 27.0f));
  drawCube(touchlines, vec3(0.96f, 0.96f, 0.98f), modelLoc, colorLoc);

  mat4 touchInner = translate(mat4(1.0f), vec3(0.0f, -5.70f, 33.0f))
                    * scale(mat4(1.0f), vec3(30.6f, 0.04f, 26.6f));
  drawCube(touchInner, vec3(0.23f, 0.57f, 0.21f), modelLoc, colorLoc);

  // White Halfway line across pitch (z = 33.0f)
  mat4 halfLine = translate(mat4(1.0f), vec3(0.0f, -5.69f, 33.0f))
                  * scale(mat4(1.0f), vec3(30.6f, 0.03f, 0.16f));
  drawCube(halfLine, vec3(0.96f, 0.96f, 0.98f), modelLoc, colorLoc);

  // Center kickoff circle
  mat4 centerCircleOuter = translate(mat4(1.0f), vec3(0.0f, -5.68f, 33.0f))
                           * scale(mat4(1.0f), vec3(4.8f, 0.04f, 4.8f));
  drawCylinder(centerCircleOuter, vec3(0.96f, 0.96f, 0.98f), modelLoc, colorLoc);

  mat4 centerCircleInner = translate(mat4(1.0f), vec3(0.0f, -5.67f, 33.0f))
                           * scale(mat4(1.0f), vec3(4.5f, 0.05f, 4.5f));
  drawCylinder(centerCircleInner, vec3(0.24f, 0.58f, 0.22f), modelLoc, colorLoc);

  // Central Clay Cricket Pitch Strip
  mat4 cricketWicket = translate(mat4(1.0f), vec3(0.0f, -5.66f, 33.0f))
                       * scale(mat4(1.0f), vec3(1.8f, 0.05f, 9.0f));
  drawCube(cricketWicket, vec3(0.78f, 0.70f, 0.52f), modelLoc, colorLoc);

  // Cricket Stumps at both ends of wicket
  float stumpZ[2] = {28.8f, 37.2f};
  for (int st = 0; st < 2; st++) {
    for (int p = -1; p <= 1; p++) {
      mat4 stump = translate(mat4(1.0f), vec3(p * 0.12f, -5.35f, stumpZ[st]))
                   * scale(mat4(1.0f), vec3(0.035f, 0.65f, 0.035f));
      drawCylinder(stump, vec3(0.85f, 0.68f, 0.38f), modelLoc, colorLoc);
    }
  }

  // Football Goals on Near and Far Ends of the Field
  float goalZ[2] = {20.0f, 46.0f};
  for (int g = 0; g < 2; g++) {
    // Goal Posts
    float postX[2] = {-2.4f, 2.4f};
    for (int p = 0; p < 2; p++) {
      mat4 gPost = translate(mat4(1.0f), vec3(postX[p], -4.85f, goalZ[g]))
                   * scale(mat4(1.0f), vec3(0.12f, 1.80f, 0.12f));
      drawCube(gPost, vec3(0.98f, 0.98f, 0.98f), modelLoc, colorLoc);
    }
    // Goal Crossbar
    mat4 gCross = translate(mat4(1.0f), vec3(0.0f, -3.95f, goalZ[g]))
                  * scale(mat4(1.0f), vec3(4.92f, 0.12f, 0.12f));
    drawCube(gCross, vec3(0.98f, 0.98f, 0.98f), modelLoc, colorLoc);

    // Goal Net Enclosure
    float netDir = (g == 0) ? -0.8f : 0.8f;
    mat4 gNet = translate(mat4(1.0f), vec3(0.0f, -4.85f, goalZ[g] + netDir))
                * scale(mat4(1.0f), vec3(4.80f, 1.70f, 1.2f));
    drawCube(gNet, vec3(0.88f, 0.92f, 0.95f), modelLoc, colorLoc);
  }

  // 4 Stadium Floodlight Towers at Field Corners
  float lightX[4] = {-19.0f, -19.0f, 19.0f, 19.0f};
  float lightZ[4] = {16.5f, 49.5f, 49.5f, 16.5f};
  for (int l = 0; l < 4; l++) {
    bool isNear = (lightZ[l] < 33.0f);
    // Near side (Our wing / South) has 5 floors, Far side (Reverse wing / North) has 2 floors
    float mastHeight = isNear ? 14.5f : 8.5f;
    float mastCenterY = isNear ? 1.40f : -1.60f;
    float headY = isNear ? 8.65f : 2.65f;

    // Steel mast
    mat4 mast = translate(mat4(1.0f), vec3(lightX[l], mastCenterY, lightZ[l]))
                * scale(mat4(1.0f), vec3(0.35f, mastHeight, 0.35f));
    drawCube(mast, vec3(0.68f, 0.70f, 0.74f), modelLoc, colorLoc);

    // Floodlight Head Array Bar
    mat4 headBar = translate(mat4(1.0f), vec3(lightX[l], headY, lightZ[l]))
                   * scale(mat4(1.0f), vec3(1.8f, 0.45f, 0.45f));
    drawCube(headBar, vec3(0.30f, 0.32f, 0.36f), modelLoc, colorLoc);

    // 4 High-Intensity Floodlight Projectors
    for (int lp = -1; lp <= 2; lp++) {
      mat4 lamp = translate(mat4(1.0f), vec3(lightX[l] + (lp - 0.5f) * 0.40f, headY, lightZ[l] + (isNear ? 0.22f : -0.22f)))
                  * scale(mat4(1.0f), vec3(0.28f, 0.28f, 0.18f));
      setEmissive(true);
      drawCube(lamp, vec3(1.0f, 0.98f, 0.82f), modelLoc, colorLoc);
      setEmissive(false);
    }
  }

  // Sideline players dugouts with canopy along near sideline (z = 16.0f)
  float benchX[2] = {-6.0f, 6.0f};
  for (int b = 0; b < 2; b++) {
    // Dugout shelter canopy
    mat4 shelter = translate(mat4(1.0f), vec3(benchX[b], -4.70f, 16.2f))
                   * scale(mat4(1.0f), vec3(3.6f, 2.0f, 1.4f));
    drawCube(shelter, vec3(0.20f, 0.45f, 0.70f), modelLoc, colorLoc);

    // Bench inside dugout
    mat4 seat = translate(mat4(1.0f), vec3(benchX[b], -5.30f, 16.2f))
                * scale(mat4(1.0f), vec3(3.2f, 0.15f, 0.6f));
    drawCube(seat, vec3(0.85f, 0.85f, 0.88f), modelLoc, colorLoc);
  }

  // --- 9. D-SHAPED RESIDENTIAL HALL BUILDING ---
  // The other 2 sides (Left & Right) have exactly 2 FLOORS.
  // The reverse opposite side (North Wing) has exactly 2 FLOORS.
  // Our side (South Wing) has exactly 5 FLOORS.
  vec3 hallWallCol = vec3(0.82f, 0.78f, 0.70f);      // University collegiate sandstone
  vec3 hallTrimCol = vec3(0.58f, 0.28f, 0.18f);      // Terracotta brick accent ledges
  vec3 hallWindowCol = vec3(0.26f, 0.48f, 0.68f);    // Reflective glass
  vec3 hallFrameCol = vec3(0.92f, 0.92f, 0.94f);     // White window casing
  vec3 hallRoofCol = vec3(0.34f, 0.35f, 0.38f);      // Dark slate roof cornice

  // Elevation tables for each wing
  float sideFloorY[2] = {-4.30f, -1.95f};                            // 2 Floors on Left & Right wings
  float northFloorY[2] = {-4.30f, -1.95f};                           // 2 Floors on Reverse/Opposite hall wing
  float southFloorY[5] = {-4.30f, -1.95f, 0.40f, 2.75f, 5.10f};     // 5 Floors on Our Room's wing (South wing)

  // ==========================================
  // ARM 1: LEFT WING OF THE HALL (WEST WING, x = -24.0f) - 2 FLOORS TOTAL
  // Inner courtyard facade at x = -21.50f, stretches from z = 4.10f to z = 55.00f
  // ==========================================
  // Main building structural body (height 6.20f, from y = -5.85f to roof at y = 0.35f)
  mat4 leftArm = translate(mat4(1.0f), vec3(-24.0f, -2.75f, 29.55f))
                 * scale(mat4(1.0f), vec3(5.0f, 6.20f, 50.9f));
  drawCube(leftArm, hallWallCol, modelLoc, colorLoc);

  // Roof parapet cornice (sits strictly ON TOP of wall from y = 0.35f to 0.70f)
  mat4 lCornice = translate(mat4(1.0f), vec3(-24.0f, 0.525f, 29.55f))
                  * scale(mat4(1.0f), vec3(5.30f, 0.35f, 51.1f));
  drawCube(lCornice, hallRoofCol, modelLoc, colorLoc);

  // 2 Horizontal terracotta floor division ledges facing courtyard (x = -21.45f)
  for (int f = 0; f < 2; f++) {
    mat4 lBand = translate(mat4(1.0f), vec3(-21.45f, sideFloorY[f] + 1.25f, 29.55f))
                 * scale(mat4(1.0f), vec3(0.10f, 0.15f, 40.9f));
    drawCube(lBand, hallTrimCol, modelLoc, colorLoc);
  }

  // 2 Stories of Balconies & French Balcony Doors on Left Wing (w = 0..4)
  float lWinZ[5] = {17.5f, 23.5f, 29.5f, 35.5f, 41.5f};
  for (int w = 0; w < 5; w++) {
    float wz = lWinZ[w];
    for (int f = 0; f < 2; f++) {
      float wy = sideFloorY[f];

      // Small narrow collegiate wooden balcony door
      drawBalconyDoor(vec3(-21.48f, wy, wz), 1.15f, 1.90f, 2, modelLoc, colorLoc);

      // Architectural Balcony with identical styling to Room 304
      drawWingBalcony(vec3(-20.85f, wy, wz), 2.50f, 1.20f, 2, modelLoc, colorLoc, (w + f * 5));
    }
  }

  // ==========================================
  // ARM 2: RIGHT WING OF THE HALL (EAST WING, x = +24.0f) - 2 FLOORS TOTAL
  // Inner courtyard facade at x = +21.50f, stretches from z = 4.10f to z = 55.00f
  // ==========================================
  mat4 rightArm = translate(mat4(1.0f), vec3(24.0f, -2.75f, 29.55f))
                  * scale(mat4(1.0f), vec3(5.0f, 6.20f, 50.9f));
  drawCube(rightArm, hallWallCol, modelLoc, colorLoc);

  mat4 rCornice = translate(mat4(1.0f), vec3(24.0f, 0.525f, 29.55f))
                  * scale(mat4(1.0f), vec3(5.30f, 0.35f, 51.1f));
  drawCube(rCornice, hallRoofCol, modelLoc, colorLoc);

  // 2 Horizontal floor division ledges facing courtyard (x = +21.45f)
  for (int f = 0; f < 2; f++) {
    mat4 rBand = translate(mat4(1.0f), vec3(21.45f, sideFloorY[f] + 1.25f, 29.55f))
                 * scale(mat4(1.0f), vec3(0.10f, 0.15f, 40.9f));
    drawCube(rBand, hallTrimCol, modelLoc, colorLoc);
  }

  // 2 Stories of Balconies & French Balcony Doors on Right Wing
  for (int w = 0; w < 5; w++) {
    float wz = lWinZ[w];
    for (int f = 0; f < 2; f++) {
      float wy = sideFloorY[f];

      // Small narrow collegiate wooden balcony door
      drawBalconyDoor(vec3(21.48f, wy, wz), 1.15f, 1.90f, 3, modelLoc, colorLoc);

      // Architectural Balcony with identical styling to Room 304
      drawWingBalcony(vec3(20.85f, wy, wz), 2.50f, 1.20f, 3, modelLoc, colorLoc, (w + f * 5 + 1));
    }
  }

  // ==========================================
  // ARM 3: OPPOSITE / REVERSE WING OF THE HALL (NORTH WING, z = 52.5f) - 2 FLOORS TOTAL
  // Inner courtyard facade at z = 50.00f, spans x from -26.50f to +26.50f
  // Seamlessly joins with Left & Right Wings at identical height y = 0.35f
  // ==========================================
  // Main building structural body (height 6.20f, from y = -5.85f to roof at y = 0.35f)
  mat4 farArm = translate(mat4(1.0f), vec3(0.0f, -2.75f, 52.5f))
                * scale(mat4(1.0f), vec3(53.0f, 6.20f, 5.0f));
  drawCube(farArm, hallWallCol, modelLoc, colorLoc);

  // Roof parapet cornice (sits strictly ON TOP of wall from y = 0.35f to 0.70f)
  mat4 fCornice = translate(mat4(1.0f), vec3(0.0f, 0.525f, 52.5f))
                  * scale(mat4(1.0f), vec3(53.3f, 0.35f, 5.3f));
  drawCube(fCornice, hallRoofCol, modelLoc, colorLoc);

  // 2 Horizontal floor division ledges on Reverse Wing (from x = -21.50f to +21.50f)
  for (int f = 0; f < 2; f++) {
    mat4 fBand = translate(mat4(1.0f), vec3(0.0f, northFloorY[f] + 1.25f, 49.95f))
                 * scale(mat4(1.0f), vec3(43.0f, 0.15f, 0.10f));
    drawCube(fBand, hallTrimCol, modelLoc, colorLoc);
  }

  // Ground Floor Grand Central Archway Gateway
  mat4 archway = translate(mat4(1.0f), vec3(0.0f, -4.30f, 52.5f))
                 * scale(mat4(1.0f), vec3(5.6f, 3.1f, 5.2f));
  drawCube(archway, vec3(0.18f, 0.18f, 0.22f), modelLoc, colorLoc);

  mat4 archHeader = translate(mat4(1.0f), vec3(0.0f, -2.75f, 49.85f))
                    * scale(mat4(1.0f), vec3(6.2f, 0.40f, 0.35f));
  drawCube(archHeader, hallTrimCol, modelLoc, colorLoc);

  // 2 Stories of Balconies & French Balcony Doors across Reverse Wing (generously clear of corners)
  float fWinX[6] = {-14.5f, -8.7f, -2.9f, 2.9f, 8.7f, 14.5f};
  for (int w = 0; w < 6; w++) {
    float wx = fWinX[w];
    for (int f = 0; f < 2; f++) {
      if (f == 0 && (w == 2 || w == 3)) continue; // Keep archway clear
      float wy = northFloorY[f];

      // Small narrow collegiate wooden balcony door
      drawBalconyDoor(vec3(wx, wy, 49.95f), 1.15f, 1.90f, 1, modelLoc, colorLoc);

      // Architectural Balcony matching Room 304 styling
      drawWingBalcony(vec3(wx, wy, 49.35f), 2.50f, 1.20f, 1, modelLoc, colorLoc, (w + f * 6 + 2));
    }
  }

  // Central Clock Tower / Hall Pediment on 2-Floor Reverse Wing Roof
  mat4 pediment = translate(mat4(1.0f), vec3(0.0f, 1.50f, 52.5f))
                  * scale(mat4(1.0f), vec3(8.0f, 1.6f, 5.0f));
  drawCube(pediment, hallWallCol, modelLoc, colorLoc);

  mat4 clockFace = translate(mat4(1.0f), vec3(0.0f, 1.50f, 49.85f))
                   * scale(mat4(1.0f), vec3(1.3f, 1.3f, 0.15f));
  drawCylinder(clockFace, vec3(0.98f, 0.98f, 0.95f), modelLoc, colorLoc);

  mat4 flagpole = translate(mat4(1.0f), vec3(0.0f, 3.00f, 52.5f))
                  * scale(mat4(1.0f), vec3(0.10f, 1.4f, 0.10f));
  drawCylinder(flagpole, vec3(0.85f, 0.85f, 0.88f), modelLoc, colorLoc);

  mat4 flag = translate(mat4(1.0f), vec3(0.55f, 3.40f, 52.5f))
              * scale(mat4(1.0f), vec3(0.90f, 0.50f, 0.04f));
  drawCube(flag, vec3(0.88f, 0.20f, 0.24f), modelLoc, colorLoc);

  // ==========================================
  // ARM 4: NEAR WING (OUR SIDE / SOUTH WING) - 5 FLOORS TOTAL!
  // Seamless, unbroken 5-story facade across the full 43m courtyard width (x = [-21.50f, +21.50f]).
  // ==========================================
  // 1. Lower building structural bodies (floors 1-3, from y = -5.85f to y = 3.55f, height 9.40f, center y = -1.15f)
  // Near-Left Wing building body: x from -26.50f to -8.00f (width 18.50f, center x = -17.25f)
  mat4 nearLeftArm = translate(mat4(1.0f), vec3(-17.25f, -1.15f, 6.60f))
                     * scale(mat4(1.0f), vec3(18.50f, 9.40f, 5.0f));
  drawCube(nearLeftArm, hallWallCol, modelLoc, colorLoc);

  // Near-Right Wing building body: x from +8.00f to +26.50f (width 18.50f, center x = +17.25f)
  mat4 nearRightArm = translate(mat4(1.0f), vec3(17.25f, -1.15f, 6.60f))
                      * scale(mat4(1.0f), vec3(18.50f, 9.40f, 5.0f));
  drawCube(nearRightArm, hallWallCol, modelLoc, colorLoc);

  // 2. Upper building body above Room 304 (floors 4-5, strictly from y = 3.55f to roof y = 7.45f, height 3.90f, center y = 5.50f)
  // Spans continuously across the entire width from x = -26.50f to +26.50f (width 53.0f, center x = 0.0f)
  mat4 upperSouthArm = translate(mat4(1.0f), vec3(0.0f, 5.50f, 6.60f))
                       * scale(mat4(1.0f), vec3(53.0f, 3.90f, 5.0f));
  drawCube(upperSouthArm, hallWallCol, modelLoc, colorLoc);

  // 3. Facade Cladding Walls facing courtyard (at z = 9.05f)
  // Lower Left Facade (x = -21.50f to -8.00f, height 9.40f, center y = -1.15f)
  mat4 nlFacade = translate(mat4(1.0f), vec3(-14.75f, -1.15f, 9.05f))
                  * scale(mat4(1.0f), vec3(13.50f, 9.40f, 0.10f));
  drawCube(nlFacade, hallWallCol, modelLoc, colorLoc);

  // Lower Right Facade (x = +8.00f to +21.50f, height 9.40f, center y = -1.15f)
  mat4 nrFacade = translate(mat4(1.0f), vec3(14.75f, -1.15f, 9.05f))
                  * scale(mat4(1.0f), vec3(13.50f, 9.40f, 0.10f));
  drawCube(nrFacade, hallWallCol, modelLoc, colorLoc);

  // Upper Facade (floors 4 & 5, spans continuous x = -21.50f to +21.50f, width 43.0f, height 3.90f, center y = 5.50f)
  mat4 upperFacade = translate(mat4(1.0f), vec3(0.0f, 5.50f, 9.05f))
                     * scale(mat4(1.0f), vec3(43.00f, 3.90f, 0.10f));
  drawCube(upperFacade, hallWallCol, modelLoc, colorLoc);

  // 4. Continuous Roof Cornice across entire South Wing (at y = 7.625f, width 53.3f)
  mat4 southCornice = translate(mat4(1.0f), vec3(0.0f, 7.625f, 6.60f))
                      * scale(mat4(1.0f), vec3(53.3f, 0.35f, 5.3f));
  drawCube(southCornice, hallRoofCol, modelLoc, colorLoc);

  // 5. Corner Pilaster Columns & Balcony Framing Piers
  // Inside corners where South Wing meets Side Wings (x = -21.50f and x = +21.50f)
  mat4 lPilaster = translate(mat4(1.0f), vec3(-21.50f, 0.80f, 9.12f))
                   * scale(mat4(1.0f), vec3(0.35f, 13.50f, 0.24f));
  drawCube(lPilaster, hallTrimCol * 0.90f, modelLoc, colorLoc);

  mat4 rPilaster = translate(mat4(1.0f), vec3(21.50f, 0.80f, 9.12f))
                   * scale(mat4(1.0f), vec3(0.35f, 13.50f, 0.24f));
  drawCube(rPilaster, hallTrimCol * 0.90f, modelLoc, colorLoc);

  // Balcony Architectural Framing Piers on the wall (x = -7.50f and x = -2.50f, height from y = -2.50f to 3.55f)
  // Cleanly terminates the lower wing ledges so NOTHING cuts across the balcony or doorway!
  mat4 bPierLeft = translate(mat4(1.0f), vec3(-7.50f, 0.525f, 9.12f))
                   * scale(mat4(1.0f), vec3(0.18f, 6.05f, 0.20f));
  drawCube(bPierLeft, hallTrimCol * 0.95f, modelLoc, colorLoc);

  mat4 bPierRight = translate(mat4(1.0f), vec3(-2.50f, 0.525f, 9.12f))
                    * scale(mat4(1.0f), vec3(0.18f, 6.05f, 0.20f));
  drawCube(bPierRight, hallTrimCol * 0.95f, modelLoc, colorLoc);

  // 6. HORIZONTAL TERRACOTTA DIVISION LEDGES (100% CLEAR DOORWAY & BALCONY)
  // Floor 1 (strictly beneath Room 304 and Balcony floor y = -2.50f, at y = -3.05f)
  mat4 bBand = translate(mat4(1.0f), vec3(0.0f, southFloorY[0] + 1.25f, 9.15f))
               * scale(mat4(1.0f), vec3(43.00f, 0.15f, 0.10f));
  drawCube(bBand, hallTrimCol, modelLoc, colorLoc);

  // Floors 2 & 3 (y = -0.70f and y = 1.65f):
  // Near-Left Wing: runs from west corner (x = -21.50f) to balcony left pier (x = -7.50f)
  // Center: x = -14.50f, width = 14.00f
  for (int f = 1; f < 3; f++) {
    mat4 nlLedge = translate(mat4(1.0f), vec3(-14.50f, southFloorY[f] + 1.25f, 9.15f))
                   * scale(mat4(1.0f), vec3(14.00f, 0.15f, 0.10f));
    drawCube(nlLedge, hallTrimCol, modelLoc, colorLoc);
  }

  // Near-Right Wing & Room wall right of balcony: runs from right balcony pier (x = -2.50f) to east corner (x = +21.50f)
  // Center: x = 9.50f, width = 24.00f
  // (Leaves the balcony and doorway between x = -7.50f and x = -2.50f 100% completely open and clear!)
  for (int f = 1; f < 3; f++) {
    mat4 nrLedge = translate(mat4(1.0f), vec3(9.50f, southFloorY[f] + 1.25f, 9.15f))
                   * scale(mat4(1.0f), vec3(24.00f, 0.15f, 0.10f));
    drawCube(nrLedge, hallTrimCol, modelLoc, colorLoc);
  }

  // Floors 4 & 5 (above our room, at y = 4.00f and y = 6.35f): continuous across full 43m courtyard facade
  for (int f = 3; f < 5; f++) {
    mat4 uBand = translate(mat4(1.0f), vec3(0.0f, southFloorY[f] + 1.25f, 9.15f))
                 * scale(mat4(1.0f), vec3(43.00f, 0.15f, 0.10f));
    drawCube(uBand, hallTrimCol, modelLoc, colorLoc);
  }

  // 7. Architectural Balconies & French Balcony Doors
  // Near-Left Wing (centered at x = -14.50f) - 5 Stories of Balconies & Balcony Doors
  for (int f = 0; f < 5; f++) {
    float wy = southFloorY[f];

    // Small narrow collegiate wooden balcony door
    drawBalconyDoor(vec3(-14.50f, wy, 9.15f), 1.15f, 1.90f, 0, modelLoc, colorLoc);

    // Architectural Balcony with identical styling to Room 304
    drawWingBalcony(vec3(-14.50f, wy, 9.75f), 2.50f, 1.20f, 0, modelLoc, colorLoc, f);
  }

  // Near-Right Wing (centered at x = +14.50f) - 5 Stories of Balconies & Balcony Doors
  for (int f = 0; f < 5; f++) {
    float wy = southFloorY[f];

    // Small narrow collegiate wooden balcony door
    drawBalconyDoor(vec3(14.50f, wy, 9.15f), 1.15f, 1.90f, 0, modelLoc, colorLoc);

    // Architectural Balcony with identical styling to Room 304
    drawWingBalcony(vec3(14.50f, wy, 9.75f), 2.50f, 1.20f, 0, modelLoc, colorLoc, f + 5);
  }

  // Floors 4 & 5 above Room 304: Elegant flush collegiate windows (NO intrusive hanging slabs above balcony)
  float uWinX[2] = {-5.0f, 2.5f};
  for (int w = 0; w < 2; w++) {
    for (int f = 3; f < 5; f++) {
      float wy = southFloorY[f];
      mat4 winF = translate(mat4(1.0f), vec3(uWinX[w], wy + 0.45f, 9.15f))
                  * scale(mat4(1.0f), vec3(2.4f, 1.45f, 0.18f));
      drawCube(winF, hallFrameCol, modelLoc, colorLoc);

      mat4 winG = translate(mat4(1.0f), vec3(uWinX[w], wy + 0.45f, 9.20f))
                  * scale(mat4(1.0f), vec3(2.1f, 1.25f, 0.14f));
      drawCube(winG, hallWindowCol, modelLoc, colorLoc);

      mat4 sill = translate(mat4(1.0f), vec3(uWinX[w], wy - 0.32f, 9.22f))
                  * scale(mat4(1.0f), vec3(2.6f, 0.10f, 0.24f));
      drawCube(sill, hallTrimCol, modelLoc, colorLoc);
    }
  }

  // Foundation plinth underneath our room (strictly beneath room floor y = -2.50f down to -5.85f)
  mat4 roomBasePlinth = translate(mat4(1.0f), vec3(0.0f, -4.175f, 9.05f))
                        * scale(mat4(1.0f), vec3(16.0f, 3.35f, 0.20f));
  drawCube(roomBasePlinth, hallWallCol, modelLoc, colorLoc);

  // Perimeter Walkways & Landscaping along the base of the Hall Wings
  mat4 lWalk = translate(mat4(1.0f), vec3(-19.8f, -5.80f, 31.0f))
               * scale(mat4(1.0f), vec3(2.4f, 0.04f, 41.0f));
  drawCube(lWalk, vec3(0.72f, 0.70f, 0.65f), modelLoc, colorLoc);

  mat4 rWalk = translate(mat4(1.0f), vec3(19.8f, -5.80f, 31.0f))
               * scale(mat4(1.0f), vec3(2.4f, 0.04f, 41.0f));
  drawCube(rWalk, vec3(0.72f, 0.70f, 0.65f), modelLoc, colorLoc);

  mat4 fWalk = translate(mat4(1.0f), vec3(0.0f, -5.80f, 48.8f))
               * scale(mat4(1.0f), vec3(42.0f, 0.04f, 2.4f));
  drawCube(fWalk, vec3(0.72f, 0.70f, 0.65f), modelLoc, colorLoc);

  // Near walkway along near arm
  mat4 nWalk = translate(mat4(1.0f), vec3(0.0f, -5.80f, 13.8f))
               * scale(mat4(1.0f), vec3(42.0f, 0.04f, 2.0f));
  drawCube(nWalk, vec3(0.72f, 0.70f, 0.65f), modelLoc, colorLoc);

  // Campus trees along the field perimeter
  float treeX[6] = {-19.5f, -19.5f, 19.5f, 19.5f, -10.0f, 10.0f};
  float treeZ[6] = {21.0f, 41.0f, 21.0f, 41.0f, 48.5f, 48.5f};
  for (int t = 0; t < 6; t++) {
    // Tree trunk
    mat4 trunk = translate(mat4(1.0f), vec3(treeX[t], -4.3f, treeZ[t]))
                 * scale(mat4(1.0f), vec3(0.35f, 3.1f, 0.35f));
    drawCylinder(trunk, vec3(0.36f, 0.20f, 0.10f), modelLoc, colorLoc);

    // Leafy canopy (1 clean lush sphere per tree)
    mat4 crown = translate(mat4(1.0f), vec3(treeX[t], -1.8f, treeZ[t]))
                 * scale(mat4(1.0f), vec3(2.6f, 2.4f, 2.6f));
    drawSphere(crown, vec3(0.20f, 0.56f, 0.22f), modelLoc, colorLoc);
  }

  // --- 10. EXPANSIVE CAMPUS HORIZON BASE (ELIMINATES ANY DARK VOID UNDERNEATH) ---
  mat4 campusBase = translate(mat4(1.0f), vec3(0.0f, -5.92f, 25.0f))
                    * scale(mat4(1.0f), vec3(260.0f, 0.10f, 220.0f));
  drawCube(campusBase, vec3(0.22f, 0.48f, 0.20f), modelLoc, colorLoc);

  // --- 11. OUTDOOR CELESTIAL ELEMENTS (Sun & Clouds) ---
  // Radiant Golden Sun in the sky above the hall
  setEmissive(true);
  mat4 sunCore = translate(mat4(1.0f), vec3(18.0f, 24.0f, 92.0f))
                 * scale(mat4(1.0f), vec3(4.5f, 4.5f, 0.6f));
  drawSphere(sunCore, vec3(1.0f, 0.96f, 0.65f), modelLoc, colorLoc);
  setEmissive(false);

  // Soft Cumulus Clouds across all directions (Front, Left, Right)
  float cloudPositions[6][3] = {
      {-22.0f, 24.0f, 88.0f}, // North-West high
      {4.0f, 26.0f, 89.0f},   // North center high
      {28.0f, 23.0f, 87.0f},  // North-East high
      {-75.0f, 25.0f, 25.0f}, // West sky above Left Wing
      {75.0f, 24.0f, 28.0f},  // East sky above Right Wing
      {-45.0f, 27.0f, 60.0f}  // North-West diagonal
  };
  for (int c = 0; c < 6; c++) {
    mat4 cBase = translate(mat4(1.0f), vec3(cloudPositions[c][0], cloudPositions[c][1], cloudPositions[c][2]))
                 * scale(mat4(1.0f), vec3(12.0f, 3.4f, 3.2f));
    drawSphere(cBase, vec3(0.96f, 0.97f, 0.99f), modelLoc, colorLoc);
  }

  // Flock of birds soaring high above the playground field
  float birdOffsets[5][2] = {{0.0f, 0.0f}, {-1.4f, -0.7f}, {1.4f, -0.7f}, {-2.8f, -1.4f}, {2.8f, -1.4f}};
  for (int b = 0; b < 5; b++) {
    float bx = -5.0f + birdOffsets[b][0];
    float by = 16.0f + birdOffsets[b][1];
    mat4 lWing = translate(mat4(1.0f), vec3(bx - 0.22f, by + 0.08f, 65.0f))
                 * rotate(mat4(1.0f), radians(25.0f), vec3(0.0f, 0.0f, 1.0f))
                 * scale(mat4(1.0f), vec3(0.40f, 0.05f, 0.08f));
    drawCube(lWing, vec3(0.20f, 0.24f, 0.30f), modelLoc, colorLoc);

    mat4 rWing = translate(mat4(1.0f), vec3(bx + 0.22f, by + 0.08f, 65.0f))
                 * rotate(mat4(1.0f), radians(-25.0f), vec3(0.0f, 0.0f, 1.0f))
                 * scale(mat4(1.0f), vec3(0.40f, 0.05f, 0.08f));
    drawCube(rWing, vec3(0.20f, 0.24f, 0.30f), modelLoc, colorLoc);
  }
}
