/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/OpenGLUtils.h"

#include <stdlib.h>

const GLchar* MODEL_VIEW_PROJECTION_MATRIX = "modelViewProjectionMatrix";
const GLchar* MODEL_VIEW_MATRIX = "modelViewMatrix";
const GLchar* VERTEX_POSITION = "vertexPosition";
const GLchar* VERTEX_NORMAL = "vertexNormal";
const GLchar* VERTEX_COLOR = "vertexColor";
const GLchar* VERTEX_Z_DISPLACEMENT = "vertexZDisplacement";
const GLchar* LIGHT_POSITION = "lightPosition";
const GLchar* LIGHT_INTENSITY = "lightIntensity";
const GLchar* AMBIENT_LIGHT_INTENSITY = "ambientLightIntensity";
const GLchar* SPECULAR_REFLECTION_COEFFICIENT = "specularReflectionCoefficient";
const GLchar* SHININESS = "shininess";
const GLchar* FRAGMENT_COLOR = "fragmentColor";

GLFWwindow* createOpenGLWindow(int width, int height, const char* title) {
    //init GLFW.
    if (!glfwInit()) {
        fprintf(stderr, "Error while initializing GLFW3\n");
        exit(-1);
    }

    //create window.
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Error: cannot open window with GLFW3\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    //init GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error while initializing GLEW\n");
        glfwTerminate();
        exit(-1);
    }

    //print OpenGL version.
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer = %s\n", renderer);
    printf("Supported OpenGL version = %s\n", version);

    return window;
}

GLuint createVertexBufferObject(GLuint attributeIndex, int vertexCount, int dimensionCount, float data[], GLenum usage) {
    //create vertex buffer object.
    GLuint vertexBufferObjectId;
    glGenBuffers(1, &vertexBufferObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId);

    //store vertex data in graphics card memory.
    int floatCount = vertexCount * dimensionCount;
    glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), data, usage);

    //set the attribute with the given index of the bound vertex array to point to the data in the bound vertex buffer.
    glVertexAttribPointer(attributeIndex, dimensionCount, GL_FLOAT, GL_FALSE, 0, NULL);
    //enable attribute.
    glEnableVertexAttribArray(attributeIndex);

    return vertexBufferObjectId;
}

GLuint createIndexBufferObject(int indexCount, unsigned int indices[]) {
    //create index buffer object.
    GLuint indexBufferObjectId;
    glGenBuffers(1, &indexBufferObjectId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectId);

    //store indices in graphics card memory.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    return indexBufferObjectId;
}

GLuint createShaderProgram(string vertexShaderSourceCodeString, string fragmentShaderSourceCodeString, vector<const GLchar*> attributeNames) {
    const GLchar* vertexShaderSourceCode = vertexShaderSourceCodeString.c_str();
    const GLchar* fragmentShaderSourceCode = fragmentShaderSourceCodeString.c_str();

    //create and compile shaders.
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSourceCode, NULL);
    glCompileShader(vertexShaderId);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShaderId);

    //link shaders into a shader program.
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    //link vertex shader input variables to attribute indices.
    for (int attributeIndex = 0; attributeIndex < attributeNames.size(); attributeIndex++) {
        glBindAttribLocation(programId, attributeIndex, attributeNames[attributeIndex]);
    }
    //link fragment shader output variable to color index 0.
    glBindFragDataLocation(programId, 0, FRAGMENT_COLOR);
    glLinkProgram(programId);
    glValidateProgram(programId);

    return programId;
}
