/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "model/ObjectInterface.h"
#include "shader/PhongShader.h"

#ifndef INCLUDED_BEACHBALL_H
#define INCLUDED_BEACHBALL_H

/**
 * 3D model of a beach ball.
 */
class BeachBall : public ObjectInterface {
    private:
        //physical properties.
        float mass;//in kg.
        float radius;//in m.
        float x;//in m.
        float y;//in m.
        float z;//in m.
        float vX;//in m/s.
        float vY;//in m/s.
        float vZ;//in m/s.
        mat4 modelMatrix;

        //geometry.
        const int verticalLevelOfDetail = 60;
        const int triangleStripCount = 6;
        GLuint vertexArrayObjectId;
        GLsizei vertexCountPerTriangleStrip;

        //material.
        PhongShader shader = PhongShader(0.9f, 15);
        const float triangleStripColors[3][3] = {
            {0, 0, 0.9f},//blue.
            {1, 1, 1},//white.
            {1, 1, 0},//yellow.
        };

        void updateModelMatrix();

    public:
        /**
         * Creates a beach ball with the given mass and radius centered on the given coordinates (in world space).
         */
        BeachBall(float mass, float radius, float x, float y, float z);

        /**
         * Getters and setters.
         */
        virtual float getMass();
        virtual vec3 getPosition();
        virtual void setPosition(vec3 position);
        virtual vec3 getVelocity();
        virtual void setVelocity(vec3 velocity);
        virtual BoundingBox getBoundingBox();

        /**
         * Returns the volume (in m3) of the space occupied by this object below the plane with the given z coordinate (in world space).
         */
        virtual float getVolumeBelowZ(float z);

        /**
         * Draws this object to the current OpenGL context.
         */
        virtual void draw(mat4 viewMatrix, mat4 projectionMatrix, float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[]);
};

#endif
