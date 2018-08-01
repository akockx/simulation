/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "model/SimulationBoundaries.h"

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

SimulationBoundaries::SimulationBoundaries(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->zMin = zMin;
    this->zMax = zMax;

    //create geometry.
    //vertices are in 3D, i.e. 3 coordinates together form 1 vertex.
    const GLint dimensionCount = 3;
    const int vertexCount = 8;
    //vertex coordinates (x, y, z) in model space.
    float vertices[vertexCount*dimensionCount] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    //vertex colors (r, g, b).
    float colors[vertexCount*dimensionCount] = {
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
    };

    //create vertex array object.
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);
    createVertexBufferObject(0, vertexCount, dimensionCount, vertices, GL_STATIC_DRAW);
    createVertexBufferObject(1, vertexCount, dimensionCount, colors, GL_STATIC_DRAW);

    //create index buffer object.
    vector<unsigned int> indices = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7,
    };
    indexCount = (int) indices.size();
    indexBufferObjectId = createIndexBufferObject(indexCount, &indices[0]);

    //init model matrix.
    modelMatrix = createModelMatrix((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2, 0, 0, 0, xMax - xMin, yMax - yMin, zMax - zMin);
}

BoundingBox SimulationBoundaries::getBoundingBox() {
    return BoundingBox(xMin, xMax, yMin, yMax, zMin, zMax);
}

void SimulationBoundaries::draw(mat4 viewMatrix, mat4 projectionMatrix) {
    //prepare shader.
    mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
    shader.use(modelViewProjectionMatrix);

    //draw lines.
    glBindVertexArray(vertexArrayObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectId);
    //note that this uses indexCount, not lineCount.
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
}
