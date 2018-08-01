/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"
#include "util/BoundingBox.h"

#ifndef INCLUDED_OBJECTINTERFACE_H
#define INCLUDED_OBJECTINTERFACE_H

/**
 * Interface for classes that contain a 3D model of an object that can be drawn.
 */
class ObjectInterface {
    public:
        /**
         * Draws this object to the current OpenGL context.
         */
        virtual void draw(mat4 viewMatrix, mat4 projectionMatrix, float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[]) = 0;

        /**
         * Getters and setters.
         */
        virtual float getMass() = 0;//in kg.
        virtual vec3 getPosition() = 0;//in world space.
        virtual void setPosition(vec3 position) = 0;//in world space.
        virtual vec3 getVelocity() = 0;//in world space.
        virtual void setVelocity(vec3 velocity) = 0;//in world space.
        virtual BoundingBox getBoundingBox() = 0;//in world space.

        /**
         * Returns the volume (in m3) of the space occupied by this object below the plane with the given z coordinate (in world space).
         */
        virtual float getVolumeBelowZ(float z) = 0;
};

#endif
