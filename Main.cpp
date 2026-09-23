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

// Input processing
void processInput(GLFWwindow *window) {
  float cameraSpeed = 4.0f * deltaTime;
  vec3 nextPos = cameraPos;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    nextPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    nextPos -= cameraSpeed * cameraFront;

  if (nextPos.x > -7.5f && nextPos.x < 7.5f)
    cameraPos.x = nextPos.x;
  if (nextPos.z > -8.5f && nextPos.z < 8.5f)
    cameraPos.z = nextPos.z;

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

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.15f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.Activate();

    mat4 view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mat4 proj =
        perspective(radians(45.0f), (float)fbWidth / fbHeight, 0.1f, 100.0f);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

    // 1. Architectural Room Shell (Walls, Floor, Ceiling, Doors, Windows)
    drawRoom(modelLoc, colorLoc);

    // 2. 4 Enhanced Beds with Posts, Rails, Recessed Mattress, Headboard &
    // Footboard
    vec3 bedCenters[4] = {vec3(-5.5f, -1.8f, -5.0f), vec3(-5.5f, -1.8f, 2.0f),
                          vec3(5.5f, -1.8f, -5.0f), vec3(5.5f, -1.8f, 2.0f)};
    for (int i = 0; i < 4; i++) {
      drawBed(bedCenters[i], modelLoc, colorLoc);
    }

    // 3. Pillows (CRITICAL BUG FIX: snug against headboard interior, zero back
    // panel clipping)
    for (int i = 0; i < 4; i++) {
      drawPillow(bedCenters[i] + vec3(0.0f, 0.06f, -1.39f), modelLoc, colorLoc);
    }

    // 4. 4 Study Tables
    vec3 tablePositions[4] = {
        vec3(-5.5f, -1.3f, -1.8f), vec3(-5.5f, -1.3f, 5.5f),
        vec3(5.5f, -1.3f, -1.8f), vec3(5.5f, -1.3f, 5.5f)};
    for (int i = 0; i < 4; i++) {
      drawStudyTable(tablePositions[i], modelLoc, colorLoc);
    }

    // 5. 4 Chairs
    vec3 chairPositions[4] = {
        vec3(-5.5f, -1.4f, -2.7f), vec3(-5.5f, -1.4f, 4.6f),
        vec3(5.5f, -1.4f, -2.7f), vec3(5.5f, -1.4f, 4.6f)};
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

    // 8. 4 Ceiling Fans (Completely Stationary / Static at all times)
    vec3 fanPositions[4] = {vec3(-5.5f, 3.0f, -5.0f), vec3(-5.5f, 3.0f, 2.0f),
                            vec3(5.5f, 3.0f, -5.0f), vec3(5.5f, 3.0f, 2.0f)};
    for (int i = 0; i < 4; i++) {
      drawCeilingFan(fanPositions[i], modelLoc, colorLoc);
    }

    // 9. Desktop Computers & CPU Towers:
    // With 180-degree Y rotation, all screens face directly towards the study
    // chairs front-on, key/mouse centered between screen base and chair edge,
    // and CPU tower placed beside monitor.
    vec3 pcBasePositions[4] = {
        vec3(-5.5f, -1.22f, -1.8f), vec3(-5.5f, -1.22f, 5.5f),
        vec3(5.5f, -1.22f, -1.8f), vec3(5.5f, -1.22f, 5.5f)};
    for (int i = 0; i < 4; i++) {
      drawComputer(pcBasePositions[i], 180.0f, modelLoc, colorLoc);
    }

    // 10. Wall Tube Lights (Flush Metallic Mount, Two End Caps, Glowing Tube)
    vec3 tubePositions[4] = {vec3(0.0f, 2.5f, -8.88f), vec3(0.0f, 2.5f, 8.88f),
                             vec3(-7.88f, 2.5f, 0.0f), vec3(7.88f, 2.5f, 0.0f)};
    for (int i = 0; i < 4; i++) {
      drawTubeLight(tubePositions[i], i, modelLoc, colorLoc);
    }

    // 11. Portable Charger Fan (placed on Tea Table 1)
    mat4 pFan = translate(mat4(1.0f), vec3(-1.0f, -1.60f, -0.35f));
    pFan = scale(pFan, vec3(0.28f, 0.38f, 0.28f));
    drawCube(pFan, vec3(0.2f, 0.4f, 0.9f), modelLoc, colorLoc);

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
    // pastries, tinted glasses)
    drawTableware(vec3(teaTablePos[0].x, -1.725f, teaTablePos[0].z), false,
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