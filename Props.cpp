#include "Props.h"
#include "Primitives.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// 1. PC & Desk Setup:
// Thin flat LCD screen with extruded bezel, slim vertical neck, flat
// rectangular desk base, separate keyboard plate on the desk, mouse/pad, and
// properly proportioned tower CPU.
// Uses rotationY to face directly towards the study chair.
void drawComputer(const vec3 &pos, float rotationY, GLuint modelLoc,
                  GLuint colorLoc) {
  // Hierarchical root transformation using rotationY
  mat4 root = translate(mat4(1.0f), pos);
  root = rotate(root, radians(rotationY), vec3(0.0f, 1.0f, 0.0f));

  // 1. Flat rectangular desk base
  mat4 base = root * translate(mat4(1.0f), vec3(0.0f, 0.01f, -0.08f)) *
              scale(mat4(1.0f), vec3(0.36f, 0.02f, 0.24f));
  drawCube(base, vec3(0.18f, 0.18f, 0.20f), modelLoc, colorLoc);

  // 2. Slim vertical neck
  mat4 neck = root * translate(mat4(1.0f), vec3(0.0f, 0.22f, -0.11f)) *
              scale(mat4(1.0f), vec3(0.06f, 0.42f, 0.04f));
  drawCube(neck, vec3(0.40f, 0.42f, 0.45f), modelLoc, colorLoc);

  // 3. Thin flat LCD monitor back housing
  mat4 chassis = root * translate(mat4(1.0f), vec3(0.0f, 0.48f, -0.095f)) *
                 scale(mat4(1.0f), vec3(1.00f, 0.60f, 0.03f));
  drawCube(chassis, vec3(0.15f, 0.15f, 0.17f), modelLoc, colorLoc);

  // 4. Extruded front bezel frame
  mat4 bezel = root * translate(mat4(1.0f), vec3(0.0f, 0.48f, -0.075f)) *
               scale(mat4(1.0f), vec3(0.98f, 0.58f, 0.015f));
  drawCube(bezel, vec3(0.12f, 0.12f, 0.13f), modelLoc, colorLoc);

  // 5. Recessed active LCD screen display (pointing toward +Z in local space)
  mat4 screen = root * translate(mat4(1.0f), vec3(0.0f, 0.49f, -0.068f)) *
                scale(mat4(1.0f), vec3(0.92f, 0.52f, 0.006f));
  drawCube(screen, vec3(0.16f, 0.42f, 0.64f), modelLoc, colorLoc);

  // 6. Monitor power LED at bottom-right bezel
  mat4 monLed = root * translate(mat4(1.0f), vec3(0.42f, 0.205f, -0.066f)) *
                scale(mat4(1.0f), vec3(0.018f, 0.012f, 0.004f));
  drawCube(monLed, vec3(0.20f, 0.90f, 0.70f), modelLoc, colorLoc);

  // 7. Separate keyboard plate directly centered in front of the monitor bezel,
  // space and between the screen base and chair edge (at local +Z = 0.22f)
  mat4 kbChassis = root * translate(mat4(1.0f), vec3(0.0f, 0.009f, 0.22f)) *
                   scale(mat4(1.0f), vec3(0.56f, 0.018f, 0.18f));
  drawCube(kbChassis, vec3(0.20f, 0.20f, 0.22f), modelLoc, colorLoc);

  mat4 keycaps = root * translate(mat4(1.0f), vec3(0.0f, 0.020f, 0.22f)) *
                 scale(mat4(1.0f), vec3(0.52f, 0.010f, 0.15f));
  drawCube(keycaps, vec3(0.32f, 0.32f, 0.35f), modelLoc, colorLoc);

  // 8. Mousepad and mouse to the right of keyboard, also between screen and
  // chair edge
  mat4 pad = root * translate(mat4(1.0f), vec3(0.38f, 0.005f, 0.22f)) *
             scale(mat4(1.0f), vec3(0.18f, 0.004f, 0.22f));
  drawCube(pad, vec3(0.10f, 0.11f, 0.13f), modelLoc, colorLoc);

  mat4 mouse = root * translate(mat4(1.0f), vec3(0.38f, 0.018f, 0.22f)) *
               scale(mat4(1.0f), vec3(0.07f, 0.024f, 0.11f));
  drawCube(mouse, vec3(0.25f, 0.26f, 0.28f), modelLoc, colorLoc);

  // 9. Desktop CPU tower placed to the side of the desk so it sits neatly
  // beside the monitor without obstructing the view
  mat4 cpuBody = root * translate(mat4(1.0f), vec3(0.70f, 0.30f, -0.06f)) *
                 scale(mat4(1.0f), vec3(0.28f, 0.60f, 0.54f));
  drawCube(cpuBody, vec3(0.16f, 0.16f, 0.18f), modelLoc, colorLoc);

  // Tower front panel (pointing toward +Z in local space)
  mat4 cpuFront = root * translate(mat4(1.0f), vec3(0.70f, 0.30f, 0.215f)) *
                  scale(mat4(1.0f), vec3(0.26f, 0.58f, 0.02f));
  drawCube(cpuFront, vec3(0.22f, 0.22f, 0.25f), modelLoc, colorLoc);

  // Power button / LED accent strip (pointing toward +Z in local space)
  mat4 cpuLed = root * translate(mat4(1.0f), vec3(0.70f, 0.52f, 0.227f)) *
                scale(mat4(1.0f), vec3(0.06f, 0.015f, 0.006f));
  drawCube(cpuLed, vec3(0.20f, 0.70f, 0.95f), modelLoc, colorLoc);

  // Front intake ventilation grille (pointing toward +Z in local space)
  mat4 cpuGrille = root * translate(mat4(1.0f), vec3(0.70f, 0.22f, 0.227f)) *
                   scale(mat4(1.0f), vec3(0.20f, 0.32f, 0.006f));
  drawCube(cpuGrille, vec3(0.08f, 0.08f, 0.09f), modelLoc, colorLoc);
}

// 2. Modern Dual-Band Wi-Fi Router:
// Sleek low-profile dark-slate/matte-black chassis with stepped/chamfered
// edges, 4 distinct thin vertical cylindrical antennas angled slightly backward
// and upward, and front status strip with tiny emissive green/cyan status LED
// points.
void drawRouter(const vec3 &pos, float rotationY, GLuint modelLoc,
                GLuint colorLoc) {
  mat4 root = translate(mat4(1.0f), pos);
  root = rotate(root, radians(rotationY), vec3(0.0f, 1.0f, 0.0f));

  // --- SLEEK CHASSIS ---
  // Lower base chassis (dark slate / matte black)
  mat4 botChassis = root * translate(mat4(1.0f), vec3(0.0f, 0.016f, 0.0f)) *
                    scale(mat4(1.0f), vec3(0.34f, 0.032f, 0.22f));
  drawCube(botChassis, vec3(0.12f, 0.12f, 0.14f), modelLoc, colorLoc);

  // Stepped / chamfered top plate (dark charcoal textured)
  mat4 topPlate = root * translate(mat4(1.0f), vec3(0.0f, 0.038f, -0.005f)) *
                  scale(mat4(1.0f), vec3(0.30f, 0.016f, 0.19f));
  drawCube(topPlate, vec3(0.16f, 0.16f, 0.18f), modelLoc, colorLoc);

  // Top center geometric ridge accent
  mat4 ridge = root * translate(mat4(1.0f), vec3(0.0f, 0.048f, -0.010f)) *
               scale(mat4(1.0f), vec3(0.11f, 0.006f, 0.14f));
  drawCube(ridge, vec3(0.09f, 0.09f, 0.10f), modelLoc, colorLoc);

  // --- STATUS INDICATORS ---
  // Front glossy indicator strip
  mat4 indStrip = root * translate(mat4(1.0f), vec3(0.0f, 0.024f, 0.111f)) *
                  scale(mat4(1.0f), vec3(0.20f, 0.012f, 0.004f));
  drawCube(indStrip, vec3(0.05f, 0.05f, 0.06f), modelLoc, colorLoc);

  // 5 Tiny emissive green/cyan status LED points (Power, Internet, 2.4G, 5G,
  // LAN)
  float ledX[5] = {-0.06f, -0.03f, 0.0f, 0.03f, 0.06f};
  for (int k = 0; k < 5; k++) {
    mat4 led = root * translate(mat4(1.0f), vec3(ledX[k], 0.024f, 0.113f)) *
               scale(mat4(1.0f), vec3(0.009f, 0.006f, 0.004f));
    drawCube(led, vec3(0.12f, 0.95f, 0.70f), modelLoc, colorLoc);
  }

  // --- 4 DISTINCT ANTENNAS ---
  // Attached to rear edge (z = -0.105f), angled slightly backward and upward
  float antX[4] = {-0.11f, -0.04f, 0.04f, 0.11f};
  for (int i = 0; i < 4; i++) {
    // Pivot hinge collar
    mat4 collar = root * translate(mat4(1.0f), vec3(antX[i], 0.032f, -0.105f)) *
                  scale(mat4(1.0f), vec3(0.022f, 0.022f, 0.022f));
    drawCube(collar, vec3(0.22f, 0.22f, 0.25f), modelLoc, colorLoc);

    // Antenna shaft: angled backward around X by -18 degrees, outer antennas
    // splayed
    mat4 antM = root * translate(mat4(1.0f), vec3(antX[i], 0.043f, -0.105f));
    antM = rotate(antM, radians(-18.0f), vec3(1.0f, 0.0f, 0.0f));
    if (i == 0)
      antM = rotate(antM, radians(10.0f), vec3(0.0f, 0.0f, 1.0f));
    else if (i == 3)
      antM = rotate(antM, radians(-10.0f), vec3(0.0f, 0.0f, 1.0f));

    // Shaft body using slender cylinder
    mat4 shaft = antM * translate(mat4(1.0f), vec3(0.0f, 0.11f, 0.0f)) *
                 scale(mat4(1.0f), vec3(0.008f, 0.22f, 0.008f));
    drawCylinder(shaft, vec3(0.10f, 0.10f, 0.11f), modelLoc, colorLoc);

    // Antenna tip accent
    mat4 tip = antM * translate(mat4(1.0f), vec3(0.0f, 0.225f, 0.0f)) *
               scale(mat4(1.0f), vec3(0.010f, 0.015f, 0.010f));
    drawCylinder(tip, vec3(0.32f, 0.32f, 0.35f), modelLoc, colorLoc);
  }
}

// 3. Under-Bed Trolley Bag:
// Structured rectangular luggage body, side zipper ribbing/bands, 8 corner
// protectors, recessed top pull-handle, top carry handle, and 4 bottom caster
// wheels.
void drawTrolleyBag(const vec3 &pos, const vec3 &shellColor, GLuint modelLoc,
                    GLuint colorLoc) {
  vec3 zipperCol = vec3(0.10f, 0.10f, 0.10f);
  vec3 sliderCol = vec3(0.85f, 0.85f, 0.88f);
  vec3 cornerCol = vec3(0.08f, 0.08f, 0.09f);
  vec3 handleCol = vec3(0.18f, 0.18f, 0.20f);

  // Bottom clamshell half
  mat4 botShell = translate(mat4(1.0f), pos + vec3(0.0f, -0.065f, 0.0f));
  botShell = scale(botShell, vec3(0.82f, 0.12f, 1.30f));
  drawCube(botShell, shellColor * 0.90f, modelLoc, colorLoc);

  // Top clamshell half
  mat4 topShell = translate(mat4(1.0f), pos + vec3(0.0f, 0.065f, 0.0f));
  topShell = scale(topShell, vec3(0.82f, 0.12f, 1.30f));
  drawCube(topShell, shellColor, modelLoc, colorLoc);

  // Dual molded hard-shell structural ribs on top
  mat4 rib1 = translate(mat4(1.0f), pos + vec3(0.0f, 0.13f, -0.28f));
  rib1 = scale(rib1, vec3(0.72f, 0.015f, 0.06f));
  drawCube(rib1, shellColor * 1.15f, modelLoc, colorLoc);

  mat4 rib2 = translate(mat4(1.0f), pos + vec3(0.0f, 0.13f, 0.28f));
  rib2 = scale(rib2, vec3(0.72f, 0.015f, 0.06f));
  drawCube(rib2, shellColor * 1.15f, modelLoc, colorLoc);

  // Center perimeter zipper welt band dividing clamshell halves
  mat4 zipper = translate(mat4(1.0f), pos);
  zipper = scale(zipper, vec3(0.835f, 0.025f, 1.315f));
  drawCube(zipper, zipperCol, modelLoc, colorLoc);

  // Metallic zipper pull tab
  mat4 pullTab = translate(mat4(1.0f), pos + vec3(0.42f, 0.0f, 0.10f));
  pullTab = scale(pullTab, vec3(0.02f, 0.035f, 0.04f));
  drawCube(pullTab, sliderCol, modelLoc, colorLoc);

  // 8 Molded protective corner caps / bumpers
  float cX[2] = {-0.38f, 0.38f};
  float cY[2] = {-0.10f, 0.10f};
  float cZ[2] = {-0.60f, 0.60f};

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        mat4 bumper = translate(mat4(1.0f), pos + vec3(cX[x], cY[y], cZ[z]));
        bumper = scale(bumper, vec3(0.09f, 0.07f, 0.09f));
        drawCube(bumper, cornerCol, modelLoc, colorLoc);
      }
    }
  }

  // Recessed top telescopic pull-handle well
  mat4 handleWell = translate(mat4(1.0f), pos + vec3(0.0f, 0.07f, -0.63f));
  handleWell = scale(handleWell, vec3(0.24f, 0.03f, 0.06f));
  drawCube(handleWell, vec3(0.14f, 0.14f, 0.15f), modelLoc, colorLoc);

  // Telescopic handle grip bar
  mat4 handleBar = translate(mat4(1.0f), pos + vec3(0.0f, 0.07f, -0.655f));
  handleBar = scale(handleBar, vec3(0.22f, 0.025f, 0.025f));
  drawCube(handleBar, handleCol, modelLoc, colorLoc);

  // Top rubber carry handle
  mat4 carryHandle = translate(mat4(1.0f), pos + vec3(0.0f, 0.14f, 0.0f));
  carryHandle = scale(carryHandle, vec3(0.06f, 0.025f, 0.26f));
  drawCube(carryHandle, cornerCol, modelLoc, colorLoc);

  // 4 Bottom caster wheels
  float wX[2] = {-0.36f, 0.36f};
  float wZ[2] = {-0.56f, 0.56f};
  for (int x = 0; x < 2; x++) {
    for (int z = 0; z < 2; z++) {
      mat4 wheel = translate(mat4(1.0f), pos + vec3(wX[x], -0.14f, wZ[z]));
      wheel = scale(wheel, vec3(0.05f, 0.04f, 0.05f));
      drawCube(wheel, vec3(0.06f, 0.06f, 0.06f), modelLoc, colorLoc);
    }
  }
}

// 4. Tea Table Tableware:
// Flat circular plate using scaled cylinder with raised rim, snacks/pastry, and
// tinted beverage glass.
void drawTableware(const vec3 &pos, bool isVariant, GLuint modelLoc,
                   GLuint colorLoc) {
  vec3 plateCol = vec3(0.96f, 0.96f, 0.97f);
  vec3 rimCol = vec3(0.90f, 0.91f, 0.93f);
  vec3 pastryCol = vec3(0.76f, 0.53f, 0.26f);
  vec3 glassCol =
      isVariant ? vec3(0.28f, 0.68f, 0.62f) : vec3(0.78f, 0.48f, 0.18f);
  vec3 glassRimCol =
      isVariant ? vec3(0.20f, 0.52f, 0.48f) : vec3(0.60f, 0.35f, 0.12f);

  float plateX = isVariant ? 0.15f : -0.15f;
  float plateZ = isVariant ? -0.15f : 0.15f;
  float glassX = isVariant ? -0.22f : 0.22f;
  float glassZ = isVariant ? -0.15f : 0.15f;

  // Flat circular plate (scaled cylinder primitive)
  mat4 plate = translate(mat4(1.0f), pos + vec3(plateX, 0.008f, plateZ));
  plate = scale(plate, vec3(0.22f, 0.016f, 0.22f));
  drawCylinder(plate, plateCol, modelLoc, colorLoc);

  // Inner plate depression / rim accent
  mat4 plateRim = translate(mat4(1.0f), pos + vec3(plateX, 0.014f, plateZ));
  plateRim = scale(plateRim, vec3(0.17f, 0.008f, 0.17f));
  drawCylinder(plateRim, rimCol, modelLoc, colorLoc);

  // Pastry / cookie snack on plate
  mat4 pastry = translate(mat4(1.0f), pos + vec3(plateX, 0.024f, plateZ));
  pastry = scale(pastry, vec3(0.055f, 0.018f, 0.055f));
  drawCylinder(pastry, pastryCol, modelLoc, colorLoc);

  // Tinted beverage glass
  mat4 glass = translate(mat4(1.0f), pos + vec3(glassX, 0.075f, glassZ));
  glass = scale(glass, vec3(0.065f, 0.15f, 0.065f));
  drawCylinder(glass, glassCol, modelLoc, colorLoc);

  // Glass weighted base ring
  mat4 gBase = translate(mat4(1.0f), pos + vec3(glassX, 0.008f, glassZ));
  gBase = scale(gBase, vec3(0.068f, 0.015f, 0.068f));
  drawCylinder(gBase, glassRimCol, modelLoc, colorLoc);
}
