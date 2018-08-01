/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

/**
 * Creates and returns a model matrix, using the given position, orientation and scale of the model.
 *
 * By default (no translation and no rotation) the model is positioned at the origin in world space,
 * the positive y-axis points to the front of the model, the positive x-axis points to the right side of the model
 * and the positive z-axis points to the top of the model.
 *
 * The model is rotated as follows (as seen from world space):
 * 1. roll about positive y-axis.
 * 2. pitch about positive x-axis.
 * 3. yaw about positive z-axis.
 */
mat4 createModelMatrix(float x, float y, float z, float yawInDegrees, float pitchInDegrees, float rollInDegrees, float xScale, float yScale, float zScale);

/**
 * Creates vertices and normal vectors for a sphere with radius 1, centered on the origin in model space.
 * Vertices are arranged in the form of one triangleStrip per horizontalLevelOfDetail.
 */
void createSphere(int horizontalLevelOfDetail, int verticalLevelOfDetail, vector<float> &vertices, vector<float> &normals);

/**
 * Creates vertices and normal vectors for a beach ball with radius 1, centered on the origin in model space.
 * Vertices are arranged in the form of one triangleStrip per horizontalLevelOfDetail.
 */
void createBeachBall(int horizontalLevelOfDetail, int verticalLevelOfDetail, vector<float> &vertices, vector<float> &normals);

/**
 * Creates a horizontal 2D grid with the given size and color, centered on the origin in model space.
 * Vertices are spaced equidistantly and are arranged in row-major order, starting at (x, y) = (-0.5 * xSize, -0.5 * ySize).
 */
void createHorizontal2DGrid(int rowCount, int columnCount, float xSize, float ySize, float color[], vector<float> &vertices, vector<float> &normals, vector<float> &colors);

/**
 * Returns the value of a 2D gaussian function with the given parameters for the given x and y.
 */
float gaussian(float x, float y, float alpha, float xCenter, float yCenter, float sigmaX, float sigmaY);
