#include <cmath>
#include <iostream>
#include <vector>

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "Primitives.h"
#include "Room.h"
#include "Furniture.h"
#include "Props.h"

using namespace std;
using namespace glm;

// Window dimensions
const unsigned int width = 1200;
const unsigned int height = 900;

// Camera state
vec3 cameraPos = vec3(0.0f, 0.0f, 5.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float cameraYaw = -90.0f;
float cameraPitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Balcony door state & animation
bool balconyDoorOpen = false;
float balconyDoorAngle = 0.0f;
bool oKeyPressedLast = false;

// Dynamic Fan Animation Angles
float ceilingFanAngle = 0.0f;
float tableFanBladeAngle = 0.0f;
float tableFanOscillateAngle = 0.0f;

// 4 Room Light Toggle States (Light 1 ON initially; 2, 3, 4 OFF initially)
bool light1On = true;
bool light2On = false;
bool light3On = false;
bool light4On = false;

bool key1PressedLast = false;
bool key2PressedLast = false;
bool key3PressedLast = false;
bool key4PressedLast = false;

// Input processing
void processInput(GLFWwindow *window) {
  float cameraSpeed = 4.0f * deltaTime;
  vec3 nextPos = cameraPos;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    nextPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    nextPos -= cameraSpeed * cameraFront;

  // Toggle balcony door with 'O' key
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
    if (!oKeyPressedLast) {
      balconyDoorOpen = !balconyDoorOpen;
      oKeyPressedLast = true;
      cout << "[BALCONY DOOR] "
           << (balconyDoorOpen ? "Door OPENED! You can now walk out onto the balcony."
                               : "Door CLOSED.")
           << endl;
    }
  } else {
    oKeyPressedLast = false;
  }

  // Toggle Room Light 1 with key '1' (Back Wall)
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
    if (!key1PressedLast) {
      light1On = !light1On;
      key1PressedLast = true;
      cout << "[LIGHT 1 - BACK WALL] " << (light1On ? "ON" : "OFF") << endl;
    }
  } else {
    key1PressedLast = false;
  }

  // Toggle Room Light 2 with key '2' (Front Wall)
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
    if (!key2PressedLast) {
      light2On = !light2On;
      key2PressedLast = true;
      cout << "[LIGHT 2 - FRONT WALL] " << (light2On ? "ON" : "OFF") << endl;
    }
  } else {
    key2PressedLast = false;
  }

  // Toggle Room Light 3 with key '3' (Left Wall)
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
    if (!key3PressedLast) {
      light3On = !light3On;
      key3PressedLast = true;
      cout << "[LIGHT 3 - LEFT WALL] " << (light3On ? "ON" : "OFF") << endl;
    }
  } else {
    key3PressedLast = false;
  }

  // Toggle Room Light 4 with key '4' (Right Wall)
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
    if (!key4PressedLast) {
      light4On = !light4On;
      key4PressedLast = true;
      cout << "[LIGHT 4 - RIGHT WALL] " << (light4On ? "ON" : "OFF") << endl;
    }
  } else {
    key4PressedLast = false;
  }

  // Camera boundaries & balcony access:
  // Balcony doorway is located on front wall (z = 9.0f) at x in [-5.8f, -4.2f].
  // Balcony terrace area spans x in [-7.2f, -2.8f], z in [8.5f, 13.0f].
  bool inBalconyDoorway = (nextPos.x >= -5.8f && nextPos.x <= -4.2f);
  bool currentlyOnBalcony = (cameraPos.z > 8.5f);

  if (currentlyOnBalcony) {
    // Currently out on the balcony
    if (nextPos.z > 8.5f) {
      if (nextPos.z <= 13.0f) cameraPos.z = nextPos.z;
      if (nextPos.x >= -7.2f && nextPos.x <= -2.8f) cameraPos.x = nextPos.x;
    } else {
      // Stepping back into the bedroom through the balcony door
      if (balconyDoorOpen && inBalconyDoorway) {
        cameraPos.z = nextPos.z;
        cameraPos.x = nextPos.x;
      }
    }
  } else {
    // Currently inside the bedroom
    if (nextPos.z <= 8.5f && nextPos.z >= -8.5f) {
      cameraPos.z = nextPos.z;
    } else if (nextPos.z > 8.5f) {
      // Stepping through the open door out onto the balcony
      if (balconyDoorOpen && inBalconyDoorway) {
        cameraPos.z = nextPos.z;
      }
    }

    if (nextPos.x >= -7.5f && nextPos.x <= 7.5f) {
      cameraPos.x = nextPos.x;
    }
  }

  float turnSpeed = 60.0f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraYaw -= turnSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraYaw += turnSpeed;

  vec3 direction;
  direction.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
  direction.y = sin(radians(cameraPitch));
  direction.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
  cameraFront = normalize(direction);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// ============================================================
// MAIN APPLICATION
// ============================================================
int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(
      width, height, "3D Virtual Walkthrough - Room 304 Detailed", NULL, NULL);
  if (window == NULL) {
    cout << "Failed to create window!" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  // Initialize primitive meshes (Cube, Sphere, Cylinder)
  initPrimitives();

  Shader shaderProgram("default.vert", "default.frag");
  glEnable(GL_DEPTH_TEST);

  GLuint colorLoc = glGetUniformLocation(shaderProgram.ID, "objectColor");
  GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
  GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
  GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "proj");

  // Manual Lighting & Shading Uniforms
  GLuint viewPosLoc = glGetUniformLocation(shaderProgram.ID, "viewPos");
  GLuint shininessLoc = glGetUniformLocation(shaderProgram.ID, "shininess");
  GLuint specStrengthLoc = glGetUniformLocation(shaderProgram.ID, "specularStrength");
  GLuint isEmissiveLoc = glGetUniformLocation(shaderProgram.ID, "isEmissive");
  setEmissiveUniformLoc(isEmissiveLoc);

  // Directional Light Uniforms (Outdoor Sunlight)
  GLuint dirLightDirLoc = glGetUniformLocation(shaderProgram.ID, "dirLight.direction");
  GLuint dirLightAmbLoc = glGetUniformLocation(shaderProgram.ID, "dirLight.ambient");
  GLuint dirLightDiffLoc = glGetUniformLocation(shaderProgram.ID, "dirLight.diffuse");
  GLuint dirLightSpecLoc = glGetUniformLocation(shaderProgram.ID, "dirLight.specular");

  // Point Light Uniforms (4 Indoor Fluorescent Lights + 1 Outdoor Balcony Lantern)
  GLuint pLightPosLoc[5], pLightAmbLoc[5], pLightDiffLoc[5], pLightSpecLoc[5];
  GLuint pLightConstLoc[5], pLightLinLoc[5], pLightQuadLoc[5];
  for (int i = 0; i < 5; i++) {
    string base = "pointLights[" + to_string(i) + "].";
    pLightPosLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "position").c_str());
    pLightAmbLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "ambient").c_str());
    pLightDiffLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "diffuse").c_str());
    pLightSpecLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "specular").c_str());
    pLightConstLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "constant").c_str());
    pLightLinLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "linear").c_str());
    pLightQuadLoc[i] = glGetUniformLocation(shaderProgram.ID, (base + "quadratic").c_str());
  }

  cout << "============================================================" << endl;
  cout << " 3D Room 304 Walkthrough Initialized (Phong Lighting Active)" << endl;
  cout << " Controls: W/S = Walk forward/backward, A/D = Turn camera" << endl;
  cout << " Press [O] = Open / Close the Balcony Door" << endl;
  cout << " Press [1] = Toggle Light 1 (Back Wall - ON by default)" << endl;
  cout << " Press [2] = Toggle Light 2 (Front Wall)" << endl;
  cout << " Press [3] = Toggle Light 3 (Left Wall)" << endl;
  cout << " Press [4] = Toggle Light 4 (Right Wall)" << endl;
  cout << " Look at the switch board beside Door 2 for physical switches!" << endl;
  cout << "============================================================" << endl;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    // Smooth door opening/closing animation towards target angle
    float targetDoorAngle = balconyDoorOpen ? 95.0f : 0.0f;
    balconyDoorAngle += (targetDoorAngle - balconyDoorAngle) * 5.0f * deltaTime;

    // Smooth motion on ceiling fan
    ceilingFanAngle = fmod(ceilingFanAngle + 340.0f * deltaTime, 360.0f);

    // Dual-motion table fan: continuous blade rotation + oscillating case sweep (left to right)
    tableFanBladeAngle = fmod(tableFanBladeAngle + 880.0f * deltaTime, 360.0f);
    tableFanOscillateAngle = sin((float)glfwGetTime() * 1.8f) * 42.0f;

    // Vibrant daylight sky blue clear color so sky seamlessly covers the full 360-degree view
    glClearColor(0.42f, 0.70f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.Activate();

    mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mat4 proj =
        perspective(radians(45.0f), (float)fbWidth / fbHeight, 0.1f, 300.0f);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

    // Update Camera position for specular Phong shading
    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform1f(shininessLoc, 32.0f);
    glUniform1f(specStrengthLoc, 0.35f);
    setEmissive(false);

    // Directional Sunlight (Sun high in sky, shining downward at an angle)
    glUniform3f(dirLightDirLoc, -0.4f, -1.0f, -0.5f);
    glUniform3f(dirLightAmbLoc, 0.22f, 0.22f, 0.24f);
    glUniform3f(dirLightDiffLoc, 0.65f, 0.65f, 0.60f);
    glUniform3f(dirLightSpecLoc, 0.40f, 0.40f, 0.40f);

    // 4 Indoor Room Lights: exact physical locations radiating into the room
    bool lightStates[4] = {light1On, light2On, light3On, light4On};
    vec3 tubeLightSources[4] = {
        vec3(0.0f, 2.45f, -8.60f), // Light 1: Back wall
        vec3(0.0f, 2.45f, 8.60f),  // Light 2: Front wall
        vec3(-7.60f, 2.45f, 0.0f),  // Light 3: Left wall
        vec3(7.60f, 2.45f, 0.0f)   // Light 4: Right wall
    };

    for (int i = 0; i < 4; i++) {
      glUniform3f(pLightPosLoc[i], tubeLightSources[i].x, tubeLightSources[i].y, tubeLightSources[i].z);
      if (lightStates[i]) {
        // ON: Light radiates from the physical tube light source across the room
        glUniform3f(pLightAmbLoc[i], 0.05f, 0.05f, 0.045f);
        glUniform3f(pLightDiffLoc[i], 0.85f, 0.85f, 0.78f);
        glUniform3f(pLightSpecLoc[i], 0.35f, 0.35f, 0.35f);
      } else {
        // OFF: Zero light emission
        glUniform3f(pLightAmbLoc[i], 0.0f, 0.0f, 0.0f);
        glUniform3f(pLightDiffLoc[i], 0.0f, 0.0f, 0.0f);
        glUniform3f(pLightSpecLoc[i], 0.0f, 0.0f, 0.0f);
      }
      glUniform1f(pLightConstLoc[i], 1.0f);
      glUniform1f(pLightLinLoc[i], 0.055f);
      glUniform1f(pLightQuadLoc[i], 0.009f);
    }

    // Point Light 4: Outdoor Balcony Coach Light (Warm sconce lantern)
    glUniform3f(pLightPosLoc[4], -3.85f, -0.28f, 9.30f);
    glUniform3f(pLightAmbLoc[4], 0.06f, 0.05f, 0.03f);
    glUniform3f(pLightDiffLoc[4], 0.70f, 0.60f, 0.40f);
    glUniform3f(pLightSpecLoc[4], 0.40f, 0.35f, 0.20f);
    glUniform1f(pLightConstLoc[4], 1.0f);
    glUniform1f(pLightLinLoc[4], 0.14f);
    glUniform1f(pLightQuadLoc[4], 0.07f);

    // 1. Architectural Room Shell, Doors (including openable Balcony Door), and Balcony Terrace
    drawRoom(balconyDoorAngle, modelLoc, colorLoc);

    // 2. 4 Enhanced Beds with Posts, Rails, Recessed Mattress, Headboard &
    // Footboard
    vec3 bedCenters[4] = {vec3(-5.5f, -1.8f, -5.0f), vec3(-5.5f, -1.8f, 2.0f),
                          vec3(5.5f, -1.8f, -5.0f), vec3(5.5f, -1.8f, 2.0f)};
    for (int i = 0; i < 4; i++) {
      drawBed(bedCenters[i], modelLoc, colorLoc);
    }

    // 3. Pillows (snug against headboard interior, zero back panel clipping)
    for (int i = 0; i < 4; i++) {
      drawPillow(bedCenters[i] + vec3(0.0f, 0.06f, -1.39f), modelLoc, colorLoc);
    }

    // 4. 4 Study Tables (Tabletop surface at y = -1.225f)
    vec3 tablePositions[4] = {
        vec3(-5.5f, -1.3f, -1.8f), vec3(-5.5f, -1.3f, 5.5f),
        vec3(5.5f, -1.3f, -1.8f), vec3(5.5f, -1.3f, 5.5f)};
    for (int i = 0; i < 4; i++) {
      drawStudyTable(tablePositions[i], modelLoc, colorLoc);
    }

    // 5. 4 Chairs (Sitting plane set lower than table surface at y = -1.70f for realistic ergonomic clearance)
    vec3 chairPositions[4] = {
        vec3(-5.5f, -1.70f, -2.7f), vec3(-5.5f, -1.70f, 4.6f),
        vec3(5.5f, -1.70f, -2.7f), vec3(5.5f, -1.70f, 4.6f)};
    for (int i = 0; i < 4; i++) {
      drawChair(chairPositions[i], modelLoc, colorLoc);
    }

    // 6. Refined Detailed Almirahs (Metal Almirah & Wooden Wardrobe)
    drawAlmirah(vec3(-7.2f, 0.0f, -7.5f), false, modelLoc,
                colorLoc); // Metal Almirah
    drawAlmirah(vec3(7.2f, 0.0f, -7.5f), true, modelLoc,
                colorLoc); // Wooden Wardrobe

    // 7. 2 Tea Tables
    vec3 teaTablePos[2] = {vec3(-1.0f, -1.8f, 0.0f), vec3(1.0f, -1.8f, 0.0f)};
    for (int i = 0; i < 2; i++) {
      drawTeaTable(teaTablePos[i], modelLoc, colorLoc);
    }

    // 8. 4 Ceiling Fans (Smooth Continuous Rotation Motion)
    vec3 fanPositions[4] = {vec3(-5.5f, 3.0f, -5.0f), vec3(-5.5f, 3.0f, 2.0f),
                            vec3(5.5f, 3.0f, -5.0f), vec3(5.5f, 3.0f, 2.0f)};
    for (int i = 0; i < 4; i++) {
      drawCeilingFan(fanPositions[i], ceilingFanAngle, modelLoc, colorLoc);
    }

    // 9. Desktop Computers & CPU Towers
    vec3 pcBasePositions[4] = {
        vec3(-5.5f, -1.22f, -1.8f), vec3(-5.5f, -1.22f, 5.5f),
        vec3(5.5f, -1.22f, -1.8f), vec3(5.5f, -1.22f, 5.5f)};
    for (int i = 0; i < 4; i++) {
      drawComputer(pcBasePositions[i], 180.0f, modelLoc, colorLoc);
    }

    // 10. Wall Tube Lights (with on/off states and physical light emission)
    vec3 tubePositions[4] = {vec3(0.0f, 2.5f, -8.88f), vec3(0.0f, 2.5f, 8.88f),
                             vec3(-7.88f, 2.5f, 0.0f), vec3(7.88f, 2.5f, 0.0f)};
    for (int i = 0; i < 4; i++) {
      drawTubeLight(tubePositions[i], i, modelLoc, colorLoc, lightStates[i]);
    }

    // 10b. 4-Gang Electrical Switch Board on Room Wall beside Door 2
    // Displays physical toggles for switches 1, 2, 3, 4 with glowing green/red LED indicators
    drawSwitchBoard(vec3(3.60f, -0.40f, -8.92f), light1On, light2On, light3On, light4On, modelLoc, colorLoc);

    // 11. Table Fan with Complex Motion on Central Tea Table 1 (positioned on left side with ample clearance)
    // Case swivels left-to-right, and propeller arms rotate 360 degrees smoothly
    drawTableFan(vec3(-1.32f, -1.725f, -0.05f), 15.0f, tableFanOscillateAngle, tableFanBladeAngle, modelLoc, colorLoc);

    // 12. Modern Dual-Band Wi-Fi Router Mounted on Wall (Right Wall at y
    // = 1.5f, z = 4.0f): Sleek dark-slate chassis, 4 backward-angled antennas,
    // front cyan status LEDs facing into room. Vertical wall plate attached
    // flush to the wall (x = 8.0f)
    mat4 wallPlate = translate(mat4(1.0f), vec3(7.94f, 1.50f, 4.0f)) *
                     scale(mat4(1.0f), vec3(0.025f, 0.18f, 0.38f));
    drawCube(wallPlate, vec3(0.25f, 0.25f, 0.28f), modelLoc, colorLoc);

    // Horizontal cradle support shelf
    mat4 cradle = translate(mat4(1.0f), vec3(7.78f, 1.45f, 4.0f)) *
                  scale(mat4(1.0f), vec3(0.30f, 0.02f, 0.36f));
    drawCube(cradle, vec3(0.20f, 0.20f, 0.22f), modelLoc, colorLoc);

    // Two support brackets underneath cradle
    float bZ[2] = {-0.12f, 0.12f};
    for (int b = 0; b < 2; b++) {
      mat4 bracket = translate(mat4(1.0f), vec3(7.85f, 1.39f, 4.0f + bZ[b])) *
                     scale(mat4(1.0f), vec3(0.15f, 0.10f, 0.025f));
      drawCube(bracket, vec3(0.22f, 0.22f, 0.25f), modelLoc, colorLoc);
    }

    // Modern Dual-Band Router mounted on wall (LEDs facing inward into the
    // room)
    drawRouter(vec3(7.76f, 1.46f, 4.0f), -90.0f, modelLoc, colorLoc);

    // 13. 4 Under-Bed Trolley Suitcases (Distinct colors, rectangular body,
    // ribs, zipper, corners, handle)
    vec3 luggageColors[4] = {
        vec3(0.14f, 0.22f, 0.38f), // Deep Navy Blue
        vec3(0.48f, 0.12f, 0.15f), // Crimson Maroon
        vec3(0.20f, 0.22f, 0.24f), // Charcoal Graphite
        vec3(0.08f, 0.30f, 0.26f)  // Forest Teal
    };
    for (int i = 0; i < 4; i++) {
      drawTrolleyBag(vec3(bedCenters[i].x, -2.35f, bedCenters[i].z + 0.15f),
                     luggageColors[i], modelLoc, colorLoc);
    }

    // 14. Detailed Tableware on Both Central Tea Tables (Circular plates,
    // pastries, tinted glasses) - positioned on right side of Table 1 with ample clearance from fan
    drawTableware(vec3(teaTablePos[0].x + 0.35f, -1.725f, teaTablePos[0].z + 0.05f), false,
                  modelLoc, colorLoc);
    drawTableware(vec3(teaTablePos[1].x, -1.725f, teaTablePos[1].z), true,
                  modelLoc, colorLoc);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup resources
  cleanupPrimitives();
  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}