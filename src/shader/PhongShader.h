/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

#ifndef INCLUDED_PHONGSHADER_H
#define INCLUDED_PHONGSHADER_H

/**
 * A shader that implements Phong shading, see https://en.wikipedia.org/wiki/Phong_shading
 */
class PhongShader {
    private:
        GLuint shaderProgramId;
        GLuint modelViewMatrixUniformIndex;
        GLuint modelViewProjectionMatrixUniformIndex;

    public:
        PhongShader(float specularReflectionCoefficient, float shininess);

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
