/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"

mat4 createModelMatrix(float x, float y, float z, float yawInDegrees, float pitchInDegrees, float rollInDegrees, float xScale, float yScale, float zScale) {
    //create scaling matrix.
    mat4 scalingMatrix = scale(mat4(1), vec3(xScale, yScale, zScale));

    //create rotation matrix.
    //roll about positive y-axis.
    mat4 rollMatrix = rotate(mat4(1), radians(rollInDegrees), vec3(0, 1, 0));
    //pitch about positive x-axis.
    mat4 pitchMatrix = rotate(mat4(1), radians(pitchInDegrees), vec3(1, 0, 0));
    //yaw about positive z-axis.
    mat4 yawMatrix = rotate(mat4(1), radians(yawInDegrees), vec3(0, 0, 1));
    //first roll, then pitch, then yaw.
    mat4 rotationMatrix = yawMatrix * pitchMatrix * rollMatrix;

    //create translation matrix.
    mat4 translationMatrix = translate(mat4(1), vec3(x, y, z));

    //first scale, then rotate, then translate.
    mat4 modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;
    return modelMatrix;
}

void createSphere(int horizontalLevelOfDetail, int verticalLevelOfDetail, vector<float> &vertices, vector<float> &normals) {
    const double deltaH = 2 * M_PI / (double) horizontalLevelOfDetail;//radians/step along horizontal small circle.

    double phi, theta;
    float x, y, z;
    int vertexIndex = 0;
    int normalIndex = 0;
    for (int h = 0; h < horizontalLevelOfDetail; h++) {
        phi = (h / (double) horizontalLevelOfDetail) * 2 * M_PI;

        //bottom vertex.
        x = 0;
        y = 0;
        z = -1;
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        normals[normalIndex++] = x;
        normals[normalIndex++] = y;
        normals[normalIndex++] = z;

        //intermediate vertices.
        for (int v = 1; v < verticalLevelOfDetail; v++) {
            theta = (v / (double) verticalLevelOfDetail - 0.5) * M_PI;

            //right vertex.
            x = (float) (cos(phi + deltaH) * cos(theta));
            y = (float) (sin(phi + deltaH) * cos(theta));
            z = (float)  sin(theta);
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;
            normals[normalIndex++] = x;
            normals[normalIndex++] = y;
            normals[normalIndex++] = z;

            //left vertex.
            x = (float) (cos(phi) * cos(theta));
            y = (float) (sin(phi) * cos(theta));
            z = (float)  sin(theta);
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;
            normals[normalIndex++] = x;
            normals[normalIndex++] = y;
            normals[normalIndex++] = z;
        }

        //top vertex.
        x = 0;
        y = 0;
        z = 1;
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        normals[normalIndex++] = x;
        normals[normalIndex++] = y;
        normals[normalIndex++] = z;
    }
}

void createBeachBall(int horizontalLevelOfDetail, int verticalLevelOfDetail, vector<float> &vertices, vector<float> &normals) {
    const double deltaH = 2 * M_PI / (double) horizontalLevelOfDetail;//radians/step along horizontal small circle.

    double phi, theta;
    float x, y, z;
    int vertexIndex = 0;
    int normalIndex = 0;
    for (int h = 0; h < horizontalLevelOfDetail; h++) {
        phi = (h / (double) horizontalLevelOfDetail) * 2 * M_PI;

        //bottom vertex.
        x = 0;
        y = 0;
        z = -1;
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        normals[normalIndex++] = x;
        normals[normalIndex++] = y;
        normals[normalIndex++] = z;

        for (int v = 1; v < verticalLevelOfDetail; v++) {
            theta = (v / (double) verticalLevelOfDetail - 0.5) * M_PI;

            //right vertex.
            x = (float) (cos(phi + deltaH) * cos(theta));
            y = (float) (sin(phi + deltaH) * cos(theta));
            z = (float)  sin(theta);
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;

            //left vertex.
            x = (float) (cos(phi) * cos(theta));
            y = (float) (sin(phi) * cos(theta));
            z = (float)  sin(theta);
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = z;

            //normals.
            x = (float) (cos(phi + 0.5 * deltaH) * cos(theta));
            y = (float) (sin(phi + 0.5 * deltaH) * cos(theta));
            z = (float)  sin(theta);
            //right normal.
            normals[normalIndex++] = x;
            normals[normalIndex++] = y;
            normals[normalIndex++] = z;
            //left normal.
            normals[normalIndex++] = x;
            normals[normalIndex++] = y;
            normals[normalIndex++] = z;
        }

        //top vertex.
        x = 0;
        y = 0;
        z = 1;
        vertices[vertexIndex++] = x;
        vertices[vertexIndex++] = y;
        vertices[vertexIndex++] = z;
        normals[normalIndex++] = x;
        normals[normalIndex++] = y;
        normals[normalIndex++] = z;
    }
}

void createHorizontal2DGrid(int rowCount, int columnCount, float xSize, float ySize, float color[], vector<float> &vertices, vector<float> &normals, vector<float> &colors) {
    float x, y;
    int vertexIndex = 0;
    int normalIndex = 0;
    int colorIndex = 0;
    for (int row = 0; row < rowCount; row++) {
        y = (row / (float) (rowCount - 1) - 0.5f) * ySize;

        for (int column = 0; column < columnCount; column++) {
            x = (column / (float) (columnCount - 1) - 0.5f) * xSize;

            //vertex.
            vertices[vertexIndex++] = x;
            vertices[vertexIndex++] = y;
            vertices[vertexIndex++] = 0;

            //normal.
            normals[normalIndex++] = 0;
            normals[normalIndex++] = 0;
            normals[normalIndex++] = 1;

            //color.
            colors[colorIndex++] = color[0];
            colors[colorIndex++] = color[1];
            colors[colorIndex++] = color[2];
        }
    }
}

float gaussian(float x, float y, float alpha, float xCenter, float yCenter, float sigmaX, float sigmaY) {
    return alpha * exp(-pow((x - xCenter) / sigmaX, 2) / 2 - pow((y - yCenter) / sigmaY, 2) / 2);
}
