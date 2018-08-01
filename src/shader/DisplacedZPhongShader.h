/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

#ifndef INCLUDED_DISPLACEDZPHONGSHADER_H
#define INCLUDED_DISPLACEDZPHONGSHADER_H

/**
 * A shader that implements Phong shading, see https://en.wikipedia.org/wiki/Phong_shading
 * Allows displacement of z coordinate per vertex.
 * This can be used for example to change the shape of a horizontal fluid surface every frame.
 */
class DisplacedZPhongShader {
    private:
        GLuint shaderProgramId;
        GLuint modelViewMatrixUniformIndex;
        GLuint modelViewProjectionMatrixUniformIndex;

    public:
        DisplacedZPhongShader(float specularReflectionCoefficient, float shininess);

        /**
         * Supply lighting information to the shader.
         */
        void setLight(float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[], mat4 viewMatrix);

        /**
         * Makes this shader "active" so that it will be used in subsequent drawing calls.
         */
        void use(mat4 modelViewMatrix, mat4 modelViewProjectionMatrix);
};

#endif
