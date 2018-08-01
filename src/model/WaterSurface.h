/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "shader/DisplacedZPhongShader.h"
#include "util/BoundingBox.h"

#ifndef INCLUDED_WATERSURFACE_H
#define INCLUDED_WATERSURFACE_H

/**
 * 3D model of a horizontal water surface.
 */
class WaterSurface {
    private:
        //physical properties.
        float xSize;//in m.
        float ySize;//in m.
        float dX;//in m.
        float dY;//in m.
        float x;//in m.
        float y;//in m.
        float z;//in m.
        mat4 modelMatrix;

        //geometry.
        const int rowCount = 100;
        const int columnCount = 100;
        //vertices are in 3D, i.e. 3 coordinates together form 1 vertex.
        const GLint dimensionCount = 3;
        const int vertexCount = rowCount * columnCount;
        vector<float> surfaceHeightValues;//vertex z displacements relative to the vertex coordinates in model space.
        vector<float> previousSurfaceHeightValues;//vertex z displacements for previous time step.
        GLuint vertexArrayObjectId;
        GLuint normalsVertexBufferObjectId;
        GLuint zDisplacementVertexBufferObjectId;
        GLuint indexBufferObjectId;
        int indexCount;

        //material.
        DisplacedZPhongShader shader = DisplacedZPhongShader(0.9f, 15);
        float waterColor[3] = {0, 0, 1};//blue.

        void updateZDisplacements();//update z displacements in graphics card memory.
        void updateNormalVectors();//update normals in graphics card memory.
        float firstDerivativeX(int row, int column);//in model space.
        float firstDerivativeY(int row, int column);//in model space.
        float secondDerivativeX(int row, int column);//in model space.
        float secondDerivativeY(int row, int column);//in model space.

    public:
        /**
         * Creates a rectangular horizontal surface with the given size centered on the given position (in world space).
         */
        WaterSurface(float xSize, float ySize, float x, float y, float z);

        /**
         * Returns the index of the vertex closest to the given x and y (in world space).
         * Returns -1 if the given coordinates are outside of this surface.
         */
        int getIndexOfClosestVertex(float x, float y);

        /**
         * Returns the height of the surface (in world space) at the given vertexIndex.
         */
        float getSurfaceHeight(int vertexIndex);

        /**
         * Returns the gradient of the surface (in world space) at the given vertexIndex.
         * The surface gradient is a 2D vector in the xy-plane in world space.
         */
        vec2 getSurfaceGradient(int vertexIndex);

        /**
         * Getters.
         */
        float getXSize();//in model space.
        float getYSize();//in model space.

        /**
         * Adds a 2D gaussian function with the given parameters to the surface height.
         * xCenter and yCenter are in model space.
         */
        void addGaussian(float alpha, float xCenter, float yCenter, float sigmaX, float sigmaY);

        /**
         * Advances physics simulation of this surface by the given deltaT (in seconds).
         */
        void advanceSimulation(float deltaT);

        /**
         * Draws this object to the current OpenGL context.
         */
        void draw(mat4 viewMatrix, mat4 projectionMatrix, float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[]);
};

#endif
