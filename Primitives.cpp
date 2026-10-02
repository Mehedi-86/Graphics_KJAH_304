#include "Primitives.h"

#include <cmath>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// ============================================================
// PRIMITIVE MESH BUFFERS (Cube, Sphere, Cylinder)
// ============================================================
static GLuint cubeVAO = 0, cubeVBO = 0, cubeEBO = 0;

static GLuint sphereVAO = 0, sphereVBO = 0, sphereEBO = 0;
static GLsizei sphereIndexCount = 0;

static GLuint cylinderVAO = 0, cylinderVBO = 0, cylinderEBO = 0;
static GLsizei cylinderIndexCount = 0;

static GLint g_isEmissiveLoc = -1;

void setEmissiveUniformLoc(GLint loc) {
  g_isEmissiveLoc = loc;
}

void setEmissive(bool emissive) {
  if (g_isEmissiveLoc != -1) {
    glUniform1i(g_isEmissiveLoc, emissive ? 1 : 0);
  }
}

// Draw helper: sets model & color uniforms, binds VAO, and draws elements
void drawMesh(GLuint vao, GLsizei count, const mat4 &model,
              const vec3 &color, GLuint modelLoc, GLuint colorLoc) {
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
  glUniform3f(colorLoc, color.r, color.g, color.b);
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void drawCube(const mat4 &model, const vec3 &color, GLuint modelLoc,
              GLuint colorLoc) {
  drawMesh(cubeVAO, 36, model, color, modelLoc, colorLoc);
}

void drawSphere(const mat4 &model, const vec3 &color, GLuint modelLoc,
                GLuint colorLoc) {
  drawMesh(sphereVAO, sphereIndexCount, model, color, modelLoc, colorLoc);
}

void drawCylinder(const mat4 &model, const vec3 &color, GLuint modelLoc,
                  GLuint colorLoc) {
  drawMesh(cylinderVAO, cylinderIndexCount, model, color, modelLoc, colorLoc);
}

// ============================================================
// MESH INITIALIZERS WITH MANUAL VERTEX NORMALS
// ============================================================

// 1. Cube Mesh with explicit face normals (24 vertices, 6 faces)
static void initCubeMesh() {
  // Format per vertex: posX, posY, posZ, normX, normY, normZ
  GLfloat vertices[] = {
      // Front Face (Normal = 0, 0, 1)
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

      // Back Face (Normal = 0, 0, -1)
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

      // Top Face (Normal = 0, 1, 0)
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,

      // Bottom Face (Normal = 0, -1, 0)
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

      // Right Face (Normal = 1, 0, 0)
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      // Left Face (Normal = -1, 0, 0)
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f
  };

  GLuint indices[] = {
       0,  1,  2,   2,  3,  0, // Front
       4,  5,  6,   6,  7,  4, // Back
       8,  9, 10,  10, 11,  8, // Top
      12, 13, 14,  14, 15, 12, // Bottom
      16, 17, 18,  18, 19, 16, // Right
      20, 21, 22,  22, 23, 20  // Left
  };

  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &cubeEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position: layout (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normal: layout (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

// 2. Sphere Mesh with smooth spherical vertex normals
static void initSphereMesh(int stacks = 24, int sectors = 32) {
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

      // Vertex position
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      // Vertex normal (for a unit sphere, normal = normalized position = (x, y, z))
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
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

  // Position: layout (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normal: layout (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

// 3. Cylinder Mesh with radial normals for sides and vertical normals for caps
static void initCylinderMesh(int sectors = 32) {
  vector<GLfloat> vertices;
  vector<GLuint> indices;
  const float PI = 3.14159265359f;

  // --- Side Body Vertices ---
  for (int j = 0; j <= sectors; ++j) {
    float angle = (float)j * 2.0f * PI / sectors;
    float x = cosf(angle);
    float z = sinf(angle);

    // Top ring (y = 0.5f), radial normal pointing outward (x, 0, z)
    vertices.push_back(x);
    vertices.push_back(0.5f);
    vertices.push_back(z);
    vertices.push_back(x);
    vertices.push_back(0.0f);
    vertices.push_back(z);

    // Bottom ring (y = -0.5f), radial normal pointing outward (x, 0, z)
    vertices.push_back(x);
    vertices.push_back(-0.5f);
    vertices.push_back(z);
    vertices.push_back(x);
    vertices.push_back(0.0f);
    vertices.push_back(z);
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

  // --- Top Cap (Normal = 0, 1, 0) ---
  int topCenterIdx = (int)vertices.size() / 6;
  vertices.push_back(0.0f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(1.0f);
  vertices.push_back(0.0f);

  int topRimStart = (int)vertices.size() / 6;
  for (int j = 0; j <= sectors; ++j) {
    float angle = (float)j * 2.0f * PI / sectors;
    vertices.push_back(cosf(angle));
    vertices.push_back(0.5f);
    vertices.push_back(sinf(angle));
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);
  }

  for (int j = 0; j < sectors; ++j) {
    indices.push_back(topCenterIdx);
    indices.push_back(topRimStart + j);
    indices.push_back(topRimStart + j + 1);
  }

  // --- Bottom Cap (Normal = 0, -1, 0) ---
  int botCenterIdx = (int)vertices.size() / 6;
  vertices.push_back(0.0f);
  vertices.push_back(-0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  vertices.push_back(-1.0f);
  vertices.push_back(0.0f);

  int botRimStart = (int)vertices.size() / 6;
  for (int j = 0; j <= sectors; ++j) {
    float angle = (float)j * 2.0f * PI / sectors;
    vertices.push_back(cosf(angle));
    vertices.push_back(-0.5f);
    vertices.push_back(sinf(angle));
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
  }

  for (int j = 0; j < sectors; ++j) {
    indices.push_back(botCenterIdx);
    indices.push_back(botRimStart + j + 1);
    indices.push_back(botRimStart + j);
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

  // Position: layout (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normal: layout (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void initPrimitives() {
  initCubeMesh();
  initSphereMesh(24, 32);
  initCylinderMesh(32);
}

void cleanupPrimitives() {
  if (cubeVAO) {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    cubeVAO = cubeVBO = cubeEBO = 0;
  }
  if (sphereVAO) {
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    sphereVAO = sphereVBO = sphereEBO = 0;
  }
  if (cylinderVAO) {
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);
    glDeleteBuffers(1, &cylinderEBO);
    cylinderVAO = cylinderVBO = cylinderEBO = 0;
  }
}
