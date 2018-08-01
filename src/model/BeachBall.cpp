/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "model/BeachBall.h"

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

BeachBall::BeachBall(float mass, float radius, float x, float y, float z) {
    this->mass = mass;
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->z = z;
    vX = 0;
    vY = 0;
    vZ = 0;

    //create geometry.
    vertexCountPerTriangleStrip = verticalLevelOfDetail * 2;
    const int vertexCount = triangleStripCount * vertexCountPerTriangleStrip;
    //vertices are in 3D, i.e. 3 coordinates together form 1 vertex.
    const GLint dimensionCount = 3;
    vector<float> vertices(vertexCount * dimensionCount);//vertex coordinates (x, y, z) in model space.
    vector<float> normals(vertexCount * dimensionCount);//vertex normal vectors (x, y, z) in model space.
    createBeachBall(triangleStripCount, verticalLevelOfDetail, vertices, normals);

    //create colors.
    vector<float> colors(vertexCount * dimensionCount);//vertex colors (r, g, b).
    //loop over triangleStripColors to give each triangle strip a single color.
    int colorIndex = 0;
    int index = 0;
    for (int p = 0; p < triangleStripCount; p++) {
        float r = triangleStripColors[colorIndex][0];
        float g = triangleStripColors[colorIndex][1];
        float b = triangleStripColors[colorIndex][2];

        for (int n = 0; n < vertexCountPerTriangleStrip; n++) {
            colors[index++] = r;
            colors[index++] = g;
            colors[index++] = b;
        }

        colorIndex = (colorIndex + 1) % size(triangleStripColors);
    }

    //create vertex array object.
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);
    createVertexBufferObject(0, vertexCount, dimensionCount, &vertices[0], GL_STATIC_DRAW);
    createVertexBufferObject(1, vertexCount, dimensionCount, &normals[0], GL_STATIC_DRAW);
    createVertexBufferObject(2, vertexCount, dimensionCount, &colors[0], GL_STATIC_DRAW);

    //init model matrix.
    updateModelMatrix();
}

void BeachBall::updateModelMatrix() {
    //(re)initialize model matrix.
    modelMatrix = createModelMatrix(x, y, z, 0, 0, 0, radius, radius, radius);
}

void BeachBall::draw(mat4 viewMatrix, mat4 projectionMatrix, float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[]) {
    updateModelMatrix();

    //prepare shader.
    shader.setLight(lightPositionInWorldSpace, lightIntensity, ambientLightIntensity, viewMatrix);
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
    shader.use(modelViewMatrix, modelViewProjectionMatrix);

    //draw triangle strips.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindVertexArray(vertexArrayObjectId);
    for (int n = 0; n < triangleStripCount; n++) {
        //note that this uses vertexCount, not triangleCount.
        glDrawArrays(GL_TRIANGLE_STRIP, n * vertexCountPerTriangleStrip, vertexCountPerTriangleStrip);
    }
}

float BeachBall::getMass() {
    return mass;
}

vec3 BeachBall::getPosition() {
    return vec3(x, y, z);
}

void BeachBall::setPosition(vec3 position) {
    x = position[0];
    y = position[1];
    z = position[2];
}

vec3 BeachBall::getVelocity() {
    return vec3(vX, vY, vZ);
}

void BeachBall::setVelocity(vec3 velocity) {
    vX = velocity[0];
    vY = velocity[1];
    vZ = velocity[2];
}

BoundingBox BeachBall::getBoundingBox() {
    return BoundingBox(x - radius, x + radius, y - radius, y + radius, z - radius, z + radius);
}

float BeachBall::getVolumeBelowZ(float z) {
    //for volume calculation approximate beach ball by a sphere with the same radius.
    float zMin = this->z - radius;
    float zMax = this->z + radius;

    if (z <= zMin) {//if beach ball is entirely above z.
        return 0;
    }

    if (z >= zMax) {//if beach ball is entirely below z.
        return (float) (4 * M_PI * pow(radius, 3) / 3);
    }

    //if beach ball is partially below z.
    float h = z - zMin;//height of spherical cap below z.
    return (float) (M_PI * h * h * (3 * radius - h) / 3);
}
