/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

extern const GLchar* MODEL_VIEW_PROJECTION_MATRIX;
extern const GLchar* MODEL_VIEW_MATRIX;
extern const GLchar* VERTEX_POSITION;
extern const GLchar* VERTEX_NORMAL;
extern const GLchar* VERTEX_COLOR;
extern const GLchar* VERTEX_Z_DISPLACEMENT;
extern const GLchar* LIGHT_POSITION;
extern const GLchar* LIGHT_INTENSITY;
extern const GLchar* AMBIENT_LIGHT_INTENSITY;
extern const GLchar* SPECULAR_REFLECTION_COEFFICIENT;
extern const GLchar* SHININESS;
extern const GLchar* FRAGMENT_COLOR;

/**
 * Creates and shows a window with the given width, height (in pixels) and title that contains an OpenGL context.
 */
GLFWwindow* createOpenGLWindow(int width, int height, const char* title);

/**
 * Creates a vertex buffer object with the given data for the attribute with the given index.
 * Returns id of created vertex buffer object.
 */
GLuint createVertexBufferObject(GLuint attributeIndex, int vertexCount, int dimensionCount, float data[], GLenum usage);

/**
 * Creates an index buffer object with the given indices.
 * Returns id of created index buffer object.
 */
GLuint createIndexBufferObject(int indexCount, unsigned int indices[]);

/**
 * Returns id of created shader program.
 */
GLuint createShaderProgram(string vertexShaderSourceCode, string fragmentShaderSourceCode, vector<const GLchar*> attributeNames);
