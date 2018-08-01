/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 *
 * Uses OpenGL 3 to render a toy model simulation of a beach ball floating on a water surface in 3D.
 * The user can press the Q, W, A and S keys to create waves.
 *
 * This program requires the following external dependencies in order to work:
 * - A graphics card that supports OpenGL version 3 or higher.
 * - Graphics Library Framework (GLFW) version 3.2.1
 * - OpenGL Extension Wrangler Library (GLEW) version 2.1.0
 * - OpenGL Mathematics (GLM) version 0.9.9.0
 */

#include <windows.h>
#include <chrono>

#include "util/OpenGLUtils.h"
#include "scene/Scene.h"

using namespace std::chrono;

static const int WINDOW_WIDTH = 1200;//in pixels.
static const int WINDOW_HEIGHT = 900;//in pixels.
static const int DESIRED_FRAME_RATE = 60;//in frames/second.
static const float DELTA_T = 1 / (float)DESIRED_FRAME_RATE;//simulation time step in seconds.

int main() {
    //create window.
    GLFWwindow* window = createOpenGLWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simulation");

    //create scene.
    Scene* scene = new Scene();

    //render loop.
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    while (!glfwWindowShouldClose(window)) {
        high_resolution_clock::time_point startTime = high_resolution_clock::now();

        //handle keyboard input.
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//escape key.
            glfwSetWindowShouldClose(window, 1);//exit.
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {//'a' key.
            scene->interact(ADD_WAVE_IN_SOUTH_WEST_CORNER_INTERACTION_TYPE);//add wave.
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {//'s' key.
            scene->interact(ADD_WAVE_IN_SOUTH_EAST_CORNER_INTERACTION_TYPE);//add wave.
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {//'q' key.
            scene->interact(ADD_WAVE_IN_NORTH_WEST_CORNER_INTERACTION_TYPE);//add wave.
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {//'w' key.
            scene->interact(ADD_WAVE_IN_NORTH_EAST_CORNER_INTERACTION_TYPE);//add wave.
        }

        //update physics.
        scene->advanceSimulation(DELTA_T);

        //render scene.
        scene->render(WINDOW_WIDTH, WINDOW_HEIGHT);

        //sleep for time remaining until next frame.
        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        //truncate calculationTime to nanoseconds.
        duration<double> calculationTime = (duration_cast<nanoseconds>) (endTime - startTime);
        double sleepTime = DELTA_T - calculationTime.count();
        if (sleepTime > 0) {
            Sleep((DWORD) (sleepTime * 1000));
        }

        //show frame.
        glfwSwapBuffers(window);
    }

    //tidy up.
    glfwTerminate();
    delete scene;

    return 0;
}
