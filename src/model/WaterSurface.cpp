/*
* Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
*/

#include "model/WaterSurface.h"

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

static const float C = 0.5f;//wave speed in m/s.
static const float D = 0.005f;//diffusion constant in m2/s.
static const float K = 10.0f;//artificial dissipation constant in s-1.

WaterSurface::WaterSurface(float xSize, float ySize, float x, float y, float z) {
    this->xSize = xSize;
    this->ySize = ySize;
    dX = xSize / (columnCount - 1);
    dY = ySize / (rowCount - 1);
    this->x = x;
    this->y = y;
    this->z = z;

    //create geometry.
    vector<float> vertices = vector<float>(vertexCount * dimensionCount);//vertex coordinates (x, y, z) in model space.
    vector<float> normals = vector<float>(vertexCount * dimensionCount);//vertex normal vectors (x, y, z) in model space.
    vector<float> colors(vertexCount * dimensionCount);//vertex colors (r, g, b).
    createHorizontal2DGrid(rowCount, columnCount, xSize, ySize, waterColor, vertices, normals, colors);
    surfaceHeightValues = vector<float>(vertexCount);//vertex z displacements relative to the vertex coordinates in model space.
    previousSurfaceHeightValues = vector<float>(vertexCount);//vertex z displacements for previous time step.
    //initialize surface heights with zero values.
    for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
        surfaceHeightValues[vertexIndex] = 0;
        previousSurfaceHeightValues[vertexIndex] = 0;
    }

    //create vertex array object.
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);
    createVertexBufferObject(0, vertexCount, dimensionCount, &vertices[0], GL_STATIC_DRAW);
    normalsVertexBufferObjectId = createVertexBufferObject(1, vertexCount, dimensionCount, &normals[0], GL_STREAM_DRAW);
    createVertexBufferObject(2, vertexCount, dimensionCount, &colors[0], GL_STATIC_DRAW);
    zDisplacementVertexBufferObjectId = createVertexBufferObject(3, vertexCount, 1, &surfaceHeightValues[0], GL_STREAM_DRAW);

    //create index buffer object.
    indexCount = (rowCount - 1) * (columnCount - 1) * 2 * 3;
    int index = 0;
    vector<unsigned int> indices(indexCount);
    for (int row = 0; row < rowCount - 1; row++) {
        for (int column = 0; column < columnCount - 1; column++) {
            int lowerLeftIndex = row * columnCount + column;
            int lowerRightIndex = lowerLeftIndex + 1;
            int upperLeftIndex = lowerLeftIndex + columnCount;
            int upperRightIndex = upperLeftIndex + 1;

            //triangle one.
            indices[index++] = lowerLeftIndex;
            indices[index++] = lowerRightIndex;
            indices[index++] = upperLeftIndex;

            //triangle two.
            indices[index++] = upperLeftIndex;
            indices[index++] = lowerRightIndex;
            indices[index++] = upperRightIndex;
        }
    }
    indexBufferObjectId = createIndexBufferObject(indexCount, &indices[0]);

    //init model matrix.
    modelMatrix = createModelMatrix(x, y, z, 0, 0, 0, 1, 1, 1);
}

void WaterSurface::updateZDisplacements() {
    //update z displacements in graphics card memory.
    glBindVertexArray(vertexArrayObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, zDisplacementVertexBufferObjectId);
    int floatCount = vertexCount;
    glBufferSubData(GL_ARRAY_BUFFER, 0, floatCount * sizeof(float), &surfaceHeightValues[0]);
}

void WaterSurface::updateNormalVectors() {
    vector<float> normals = vector<float>(vertexCount * dimensionCount);//vertex normal vectors (x, y, z) in model space.

    //calculate normals using current surfaceHeightValues.
    int normalIndex = 0;
    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            //determine tangent vector to the surface in x direction.
            vec3 tangentInXDirection = vec3(1, 0, firstDerivativeX(row, column));

            //determine tangent vector to the surface in y direction.
            vec3 tangentInYDirection = vec3(0, 1, firstDerivativeY(row, column));

            //surface normal vector = cross product of two tangent vectors.
            vec3 normal = normalize(cross(tangentInXDirection, tangentInYDirection));

            normals[normalIndex++] = normal[0];
            normals[normalIndex++] = normal[1];
            normals[normalIndex++] = normal[2];
        }
    }

    //update normals in graphics card memory.
    glBindVertexArray(vertexArrayObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVertexBufferObjectId);
    int floatCount = vertexCount * dimensionCount;
    glBufferSubData(GL_ARRAY_BUFFER, 0, floatCount * sizeof(float), &normals[0]);
}

void WaterSurface::draw(mat4 viewMatrix, mat4 projectionMatrix, float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[]) {
    updateZDisplacements();
    updateNormalVectors();

    //prepare shader.
    shader.setLight(lightPositionInWorldSpace, lightIntensity, ambientLightIntensity, viewMatrix);
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
    shader.use(modelViewMatrix, modelViewProjectionMatrix);

    //draw triangles.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glBindVertexArray(vertexArrayObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectId);
    //note that this uses indexCount, not triangleCount.
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

int WaterSurface::getIndexOfClosestVertex(float x, float y) {
    //convert x,y to model space.
    //this code assumes that this surface's model space axes have the same orientation as the corresponding world space axes.
    x -= this->x;
    y -= this->y;
    if (x < -0.5f * xSize || x > 0.5f * xSize || y < -0.5f * ySize || y > 0.5f * ySize) {//if outside of surface.
        return -1;
    }

    //find nearest row and column.
    int row = (int) round((y / ySize + 0.5f) * (rowCount - 1));
    int column = (int) round((x / xSize + 0.5f) * (columnCount - 1));

    //determine corresponding vertexIndex.
    return row * columnCount + column;
}

float WaterSurface::getSurfaceHeight(int vertexIndex) {
    //convert surface height to world space.
    //this code assumes that this surface's model space axes have the same orientation as the corresponding world space axes.
    return z + surfaceHeightValues[vertexIndex];
}

vec2 WaterSurface::getSurfaceGradient(int vertexIndex) {
    int row = vertexIndex / columnCount;
    int column = vertexIndex % columnCount;
    //this code assumes that this surface's model space axes have the same orientation as the corresponding world space axes.
    return vec2(firstDerivativeX(row, column), firstDerivativeY(row, column));
}

float WaterSurface::getXSize() {
    return xSize;
}

float WaterSurface::getYSize() {
    return ySize;
}

void WaterSurface::addGaussian(float alpha, float xCenter, float yCenter, float sigmaX, float sigmaY) {
    //add a gaussian function with the given parameters to surfaceHeightValues.
    float y = -0.5f * ySize;
    int vertexIndex = 0;
    for (int row = 0; row < rowCount; row++) {
        float x = -0.5f * xSize;

        for (int column = 0; column < columnCount; column++) {
            float value = gaussian(x, y, alpha, xCenter, yCenter, sigmaX, sigmaY);
            surfaceHeightValues[vertexIndex] += value;
            //add the same values to previousSurfaceHeightValues for numerical consistency in the simulation.
            //Otherwise the temporal terms in the finite-difference approximation will be messed up.
            previousSurfaceHeightValues[vertexIndex] += value;
            vertexIndex++;

            x += dX;
        }

        y += dY;
    }
}

float WaterSurface::firstDerivativeX(int row, int column) {
    int i = row * columnCount + column;

    //calculate first derivative of surface height in x direction at the given row and column.
    if (column == 0) {//if western edge.
        //forward difference approximation (first-order accurate).
        return (surfaceHeightValues[i + 1] - surfaceHeightValues[i]) / dX;

    } else if (column == columnCount - 1) {//if eastern edge.
        //backward difference approximation (first-order accurate).
        return (surfaceHeightValues[i] - surfaceHeightValues[i - 1]) / dX;

    } else {
        //central difference approximation (second-order accurate).
        return (surfaceHeightValues[i + 1] - surfaceHeightValues[i - 1]) / (2 * dX);
    }
}

float WaterSurface::firstDerivativeY(int row, int column) {
    int i = row * columnCount + column;

    //calculate first derivative of surface height in y direction at the given row and column.
    if (row == 0) {//if southern edge.
        //forward difference approximation (first-order accurate).
        return (surfaceHeightValues[i + columnCount] - surfaceHeightValues[i]) / dY;

    } else if (row == rowCount - 1) {//if northern edge.
        //backward difference approximation (first-order accurate).
        return (surfaceHeightValues[i] - surfaceHeightValues[i - columnCount]) / dY;

    } else {
        //central difference approximation (second-order accurate).
        return (surfaceHeightValues[i + columnCount] - surfaceHeightValues[i - columnCount]) / (2 * dY);
    }
}

float WaterSurface::secondDerivativeX(int row, int column) {
    int i = row * columnCount + column;

    //calculate second derivative of surface height in x direction at the given row and column.
    if (column == 0) {//if western edge.
        //finite-difference approximation (first-order accurate).
        return (- 2 * surfaceHeightValues[i] + 4 * surfaceHeightValues[i + 1] - 2 * surfaceHeightValues[i + 2]) / (dX * dX);

    } else if (column == columnCount - 1) {//if eastern edge.
        //finite-difference approximation (first-order accurate).
        return (- 2 * surfaceHeightValues[i - 2] + 4 * surfaceHeightValues[i - 1] - 2 * surfaceHeightValues[i]) / (dX * dX);

    } else {
        //finite-difference approximation (second-order accurate).
        return (surfaceHeightValues[i + 1] - 2 * surfaceHeightValues[i] + surfaceHeightValues[i - 1]) / (dX * dX);
    }
}

float WaterSurface::secondDerivativeY(int row, int column) {
    int i = row * columnCount + column;

    //calculate second derivative of surface height in y direction at the given row and column.
    if (row == 0) {//if southern edge.
        //finite-difference approximation (first-order accurate).
        return (- 2 * surfaceHeightValues[i] + 4 * surfaceHeightValues[i + columnCount] - 2 * surfaceHeightValues[i + 2 * columnCount]) / (dY * dY);

    } else if (row == rowCount - 1) {//if northern edge.
        //finite-difference approximation (first-order accurate).
        return (- 2 * surfaceHeightValues[i - 2 * columnCount] + 4 * surfaceHeightValues[i - columnCount] - 2 * surfaceHeightValues[i]) / (dY * dY);

    } else {
        //finite-difference approximation (second-order accurate).
        return (surfaceHeightValues[i + columnCount] - 2 * surfaceHeightValues[i] + surfaceHeightValues[i - columnCount]) / (dY * dY);
    }
}

void WaterSurface::advanceSimulation(float deltaT) {
    vector<float> newSurfaceHeightValues = vector<float>(vertexCount);//temporary vector to store calculated values.

    //this code solves the 2D second-order wave equation numerically using an explicit euler method
    //with finite-difference approximations for both the spatial and the temporal derivatives.
    for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
        int row = vertexIndex / columnCount;
        int column = vertexIndex % columnCount;
        float previousZ = previousSurfaceHeightValues[vertexIndex];
        float currentZ = surfaceHeightValues[vertexIndex];

        //linear advection equation.
        //float artificialDissipationTerm = K * (deltaX * deltaX * secondDerivativeX(row, column) + deltaY * deltaY * secondDerivativeY(row, column));
        //float spatialTerms = C * (firstDerivativeX(row, column) + firstDerivativeY(row, column)) - artificialDissipationTerm;
        //float nextZ = currentZ - deltaT * spatialTerms;

        //linear diffusion equation.
        //float spatialTerms = - D * (secondDerivativeX(row, column) + secondDerivativeY(row, column));
        //float nextZ = currentZ - deltaT * spatialTerms;

        //linear advection-diffusion equation.
        //float spatialTerms = C * (firstDerivativeX(row, column) + firstDerivativeY(row, column)) - D * (secondDerivativeX(row, column) + secondDerivativeY(row, column));
        //float nextZ = currentZ - deltaT * spatialTerms;

        //second-order wave equation.
        float spatialTerms = - C * C * (secondDerivativeX(row, column) + secondDerivativeY(row, column));
        float nextZ = 2 * currentZ - previousZ - deltaT * deltaT * spatialTerms;

        newSurfaceHeightValues[vertexIndex] = nextZ;
    }

    //set boundary values equal to adjacent values to avoid phase jump for waves reflecting at the boundaries.
    for (int row = 0; row < rowCount; row++) {
        //western edge.
        int i = row * columnCount;
        newSurfaceHeightValues[i] = newSurfaceHeightValues[i + 1];
        //eastern edge.
        i = row * columnCount + columnCount - 1;
        newSurfaceHeightValues[i] = newSurfaceHeightValues[i - 1];
    }
    for (int column = 0; column < columnCount; column++) {
        //southern edge.
        int i = column;
        newSurfaceHeightValues[i] = newSurfaceHeightValues[i + columnCount];
        //northern edge.
        i = (rowCount - 1) * columnCount + column;
        newSurfaceHeightValues[i] = newSurfaceHeightValues[i - columnCount];
    }

    for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
        previousSurfaceHeightValues[vertexIndex] = surfaceHeightValues[vertexIndex];
        surfaceHeightValues[vertexIndex] = newSurfaceHeightValues[vertexIndex];
    }
}
