/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "shader/PhongShader.h"
#include "util/FileUtils.h"

PhongShader::PhongShader(float specularReflectionCoefficient, float shininess) {
    //this code assumes that the shader files are located in a folder called "shaders" next to the bin folder.
    //The current working directory should be e.g. bin/x64/
    string vertexShaderSourceCode = readFile("../../shaders/phong_vertex_shader.glsl");
    string fragmentShaderSourceCode = readFile("../../shaders/phong_fragment_shader.glsl");

    vector<const GLchar*> attributeNames = {VERTEX_POSITION, VERTEX_NORMAL, VERTEX_COLOR};
    shaderProgramId = createShaderProgram(vertexShaderSourceCode, fragmentShaderSourceCode, attributeNames);

    modelViewMatrixUniformIndex = glGetUniformLocation(shaderProgramId, MODEL_VIEW_MATRIX);
    modelViewProjectionMatrixUniformIndex = glGetUniformLocation(shaderProgramId, MODEL_VIEW_PROJECTION_MATRIX);

    glUseProgram(shaderProgramId);
    glUniform1f(glGetUniformLocation(shaderProgramId, SPECULAR_REFLECTION_COEFFICIENT), specularReflectionCoefficient);
    glUniform1f(glGetUniformLocation(shaderProgramId, SHININESS), shininess);
}

void PhongShader::setLight(float lightPositionInWorldSpace[], float lightIntensity[], float ambientLightIntensity[], mat4 viewMatrix) {
    vec4 lightPositionInWorldSpaceVector = vec4(lightPositionInWorldSpace[0], lightPositionInWorldSpace[1], lightPositionInWorldSpace[2], 1);
    vec4 lightPositionInCameraSpace = viewMatrix * lightPositionInWorldSpaceVector;

    glUseProgram(shaderProgramId);
    glUniform3fv(glGetUniformLocation(shaderProgramId, LIGHT_POSITION), 1, &lightPositionInCameraSpace[0]);
    glUniform3fv(glGetUniformLocation(shaderProgramId, LIGHT_INTENSITY), 1, lightIntensity);
    glUniform3fv(glGetUniformLocation(shaderProgramId, AMBIENT_LIGHT_INTENSITY), 1, ambientLightIntensity);
}

void PhongShader::use(mat4 modelViewMatrix, mat4 modelViewProjectionMatrix) {
    glUseProgram(shaderProgramId);
    glUniformMatrix4fv(modelViewMatrixUniformIndex, 1, GL_FALSE, &modelViewMatrix[0][0]);
    glUniformMatrix4fv(modelViewProjectionMatrixUniformIndex, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
}
