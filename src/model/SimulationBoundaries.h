/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/BoundingBox.h"
#include "shader/BasicShader.h"

#ifndef INCLUDED__SIMULATIONBOUNDARIES_H
#define INCLUDED__SIMULATIONBOUNDARIES_H

/**
* Box that forms the boundaries of a scene.
*/
class SimulationBoundaries {
    private:
        //physical properties.
        float xMin;//in m.
        float xMax;//in m.
        float yMin;//in m.
        float yMax;//in m.
        float zMin;//in m.
        float zMax;//in m.
        mat4 modelMatrix;

        //geometry.
        GLuint vertexArrayObjectId;
        GLuint indexBufferObjectId;
        int indexCount;

        //material.
        BasicShader shader = BasicShader();

    public:
        /**
         * Creates boundaries at the given coordinates (in world space).
         */
        SimulationBoundaries(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

        /**
        * Returns a bounding box that represents the simulation boundaries.
        */
        BoundingBox getBoundingBox();

        /**
         * Draws the boundaries to the current OpenGL context.
         */
        void draw(mat4 viewMatrix, mat4 projectionMatrix);
};

#endif
