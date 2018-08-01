/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "shader/BasicShader.h"
#include "util/FileUtils.h"

BasicShader::BasicShader() {
    //this code assumes that the shader files are located in a folder called "shaders" next to the bin folder.
    //The current working directory should be e.g. bin/x64/
    string vertexShaderSourceCode = readFile("../../shaders/basic_vertex_shader.glsl");
    string fragmentShaderSourceCode = readFile("../../shaders/basic_fragment_shader.glsl");

    vector<const GLchar*> attributeNames = {VERTEX_POSITION, VERTEX_COLOR};
    shaderProgramId = createShaderProgram(vertexShaderSourceCode, fragmentShaderSourceCode, attributeNames);

    modelViewProjectionMatrixUniformIndex = glGetUniformLocation(shaderProgramId, MODEL_VIEW_PROJECTION_MATRIX);
}

void BasicShader::use(mat4 modelViewProjectionMatrix) {
    glUseProgram(shaderProgramId);
    glUniformMatrix4fv(modelViewProjectionMatrixUniformIndex, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
}
