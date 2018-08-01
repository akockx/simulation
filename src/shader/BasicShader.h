/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"
#include "util/OpenGLUtils.h"

#ifndef INCLUDED_BASICSHADER_H
#define INCLUDED_BASICSHADER_H

/**
 * A shader with a fixed color for each vertex (no lighting).
 */
class BasicShader {
    private:
        GLuint shaderProgramId;
        GLuint modelViewProjectionMatrixUniformIndex;

    public:
        BasicShader();

        /**
         * Makes this shader "active" so that it will be used in subsequent drawing calls.
         */
        void use(mat4 modelViewProjectionMatrix);
};

#endif
