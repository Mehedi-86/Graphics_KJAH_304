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

#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"

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

// ============================================================
// PRIMITIVE MESH BUFFERS (Cube, Sphere, Cylinder)
// ============================================================
static GLuint cubeVAO = 0, cubeVBO = 0, cubeEBO = 0;

static GLuint sphereVAO = 0, sphereVBO = 0, sphereEBO = 0;
static GLsizei sphereIndexCount = 0;

static GLuint cylinderVAO = 0, cylinderVBO = 0, cylinderEBO = 0;
static GLsizei cylinderIndexCount = 0;

// Draw helper: sets model & color uniforms, binds VAO, and draws elements
inline void drawMesh(GLuint vao, GLsizei count, const mat4 &model,
                     const vec3 &color, GLuint modelLoc, GLuint colorLoc) {
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
  glUniform3f(colorLoc, color.r, color.g, color.b);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

inline void drawCube(const mat4 &model, const vec3 &color, GLuint modelLoc,
                     GLuint colorLoc) {
  drawMesh(cubeVAO, 36, model, color, modelLoc, colorLoc);
}

inline void drawSphere(const mat4 &model, const vec3 &color, GLuint modelLoc,
                       GLuint colorLoc) {
  drawMesh(sphereVAO, sphereIndexCount, model, color, modelLoc, colorLoc);
}

inline void drawCylinder(const mat4 &model, const vec3 &color, GLuint modelLoc,
                         GLuint colorLoc) {
  drawMesh(cylinderVAO, cylinderIndexCount, model, color, modelLoc, colorLoc);
}

// Mesh Initializers
void initCubeMesh() {
  GLfloat vertices[] = {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  1.0f, 0.5f,  -0.5f,
                        -0.5f, 1.0f,  1.0f,  1.0f, 0.5f,  0.5f, -0.5f, 1.0f,
                        1.0f,  1.0f,  -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  1.0f,
                        -0.5f, -0.5f, 0.5f,  1.0f, 1.0f,  1.0f, 0.5f,  -0.5f,
                        0.5f,  1.0f,  1.0f,  1.0f, 0.5f,  0.5f, 0.5f,  1.0f,
                        1.0f,  1.0f,  -0.5f, 0.5f, 0.5f,  1.0f, 1.0f,  1.0f};

  GLuint indices[] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
                      1, 5, 6, 6, 2, 1, 3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0};

  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &cubeEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void initSphereMesh(int stacks = 16, int sectors = 24) {
  vector<GLfloat> vertices;
  vector<GLuint> indices;
  const float PI = 3.14159265359f;

  for (int i = 0; i <= stacks; ++i) {
    float stackAngle = PI / 2.0f - (float)i * PI / stacks;
    float xy = cosf(stackAngle);
    float y = sinf(stackAngle);

    for (int j = 0; j <= sectors; ++j) {
      float sectorAngle = (float)j * 2.0f * PI / sectors;
      float x = xy * cosf(sectorAngle);
      float z = xy * sinf(sectorAngle);

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
    }
  }

  for (int i = 0; i < stacks; ++i) {
    int k1 = i * (sectors + 1);
    int k2 = k1 + sectors + 1;
    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }
      if (i != (stacks - 1)) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }

  sphereIndexCount = (GLsizei)indices.size();

  glGenVertexArrays(1, &sphereVAO);
  glBindVertexArray(sphereVAO);

  glGenBuffers(1, &sphereVBO);
  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &sphereEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void initCylinderMesh(int sectors = 24) {
  vector<GLfloat> vertices;
  vector<GLuint> indices;
  const float PI = 3.14159265359f;

  // Side vertices
  for (int j = 0; j <= sectors; ++j) {
    float angle = (float)j * 2.0f * PI / sectors;
    float x = cosf(angle);
    float z = sinf(angle);

    // Top ring (y = 0.5)
    vertices.push_back(x);
    vertices.push_back(0.5f);
    vertices.push_back(z);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    // Bottom ring (y = -0.5)
    vertices.push_back(x);
    vertices.push_back(-0.5f);
    vertices.push_back(z);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
  }

  for (int j = 0; j < sectors; ++j) {
    int top1 = j * 2;
    int bot1 = top1 + 1;
    int top2 = (j + 1) * 2;
    int bot2 = top2 + 1;

    indices.push_back(top1);
    indices.push_back(bot1);
    indices.push_back(top2);
    indices.push_back(top2);
    indices.push_back(bot1);
    indices.push_back(bot2);
  }

  // Top cap
  int topCenterIdx = (int)vertices.size() / 6;
  vertices.push_back(0.0f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);

  for (int j = 0; j < sectors; ++j) {
    indices.push_back(topCenterIdx);
    indices.push_back(j * 2);
    indices.push_back((j + 1) * 2);
  }

  // Bottom cap
  int botCenterIdx = (int)vertices.size() / 6;
  vertices.push_back(0.0f);
  vertices.push_back(-0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);
  vertices.push_back(1.0f);

  for (int j = 0; j < sectors; ++j) {
    indices.push_back(botCenterIdx);
    indices.push_back((j + 1) * 2 + 1);
    indices.push_back(j * 2 + 1);
  }

  cylinderIndexCount = (GLsizei)indices.size();

  glGenVertexArrays(1, &cylinderVAO);
  glBindVertexArray(cylinderVAO);

  glGenBuffers(1, &cylinderVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &cylinderEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

// ============================================================
// MODULAR 3D MODEL FUNCTIONS (PURE MODERN OPENGL)
// ============================================================

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

// 8. PC & Desk Setup:
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
  // between the screen base and the chair edge (at local +Z = 0.22f)
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

// 9. Modern Dual-Band Wi-Fi Router:
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

// 9. Detailed Architectural Door:
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

// 10. Multi-Pane Window:
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

// 11. Tube Lights:
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

// 12. Under-Bed Trolley Bag:
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

// 13. Tea Table Tableware:
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

// Room Structure: Walls, floor, ceiling, framed doors with recessed
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

  // Initialize meshes (Cube, Sphere, Cylinder)
  initCubeMesh();
  initSphereMesh(16, 24);
  initCylinderMesh(24);

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
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &cubeEBO);

  glDeleteVertexArrays(1, &sphereVAO);
  glDeleteBuffers(1, &sphereVBO);
  glDeleteBuffers(1, &sphereEBO);

  glDeleteVertexArrays(1, &cylinderVAO);
  glDeleteBuffers(1, &cylinderVBO);
  glDeleteBuffers(1, &cylinderEBO);

  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}