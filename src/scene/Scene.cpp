/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "scene/Scene.h"

#include <stdlib.h>

#include "util/OpenGLUtils.h"
#include "model/BeachBall.h"

static const float ALPHA = 0.02f;//wave height in m.
static const float SIGMA_X = 0.1f;//wave spread in x direction.
static const float SIGMA_Y = 0.1f;//wave spread in y direction.

static const float G = 9.80665f;//gravitational acceleration in m/s2.
static const float DENSITY_OF_WATER = 997.0f;//density of water at 25 degrees Celsius in kg/m3.

Scene::Scene() {
    //set clear color to black.
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    //create geometry.
    bounds = new SimulationBoundaries(-1, 1, -1, 1, 0, 1.5f);
    waterSurface = new WaterSurface(2, 2, 0, 0, 0.5f);
    objects.push_back(new BeachBall(0.1f, 0.25f, 0, 0, 1));

    //create camera.
    vec3 cameraPosition = vec3(0, -3, 1.5f);
    vec3 cameraTarget = vec3(0, 0, 0.5f);//center of water surface.
    vec3 upVector = vec3(0, 1, 0);
    viewMatrix = lookAt(cameraPosition, cameraTarget, upVector);

    int error = glGetError();
    if (error != 0) {
        fprintf(stderr, "Error during initialization\n");
        glfwTerminate();
        exit(-1);
    }
}

Scene::~Scene() {
    for (int n = 0; n < objects.size(); n++) {
        delete objects[n];
    }
    delete waterSurface;
    delete bounds;
}

void Scene::render(int width, int height) {
    //(re)initialize projection matrix.
    if (width <= 0) width = 1;//to avoid aspectRatio of zero.
    if (height <= 0) height = 1;//to avoid divide by zero.
    float aspectRatio = width / (float) height;
    mat4 projectionMatrix = perspective(radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw objects.
    bounds->draw(viewMatrix, projectionMatrix);
    waterSurface->draw(viewMatrix, projectionMatrix, lightPositionInWorldSpace, lightIntensity, ambientLightIntensity);
    for (int n = 0; n < objects.size(); n++) {
        objects[n]->draw(viewMatrix, projectionMatrix, lightPositionInWorldSpace, lightIntensity, ambientLightIntensity);
    }

    int error = glGetError();
    if (error != 0) {
        fprintf(stderr, "Error during rendering\n");
        glfwTerminate();
        exit(-1);
    }
}

void Scene::interact(int interactionType) {
    float xSize = waterSurface->getXSize();
    float ySize = waterSurface->getYSize();
    switch (interactionType) {
        case ADD_WAVE_IN_SOUTH_WEST_CORNER_INTERACTION_TYPE:
            waterSurface->addGaussian(ALPHA, -0.25f*xSize, -0.25f*ySize, SIGMA_X, SIGMA_Y);
            break;
        case ADD_WAVE_IN_SOUTH_EAST_CORNER_INTERACTION_TYPE:
            waterSurface->addGaussian(ALPHA, 0.25f*xSize, -0.25f*ySize, SIGMA_X, SIGMA_Y);
            break;
        case ADD_WAVE_IN_NORTH_WEST_CORNER_INTERACTION_TYPE:
            waterSurface->addGaussian(ALPHA, -0.25f*xSize, 0.25f*ySize, SIGMA_X, SIGMA_Y);
            break;
        case ADD_WAVE_IN_NORTH_EAST_CORNER_INTERACTION_TYPE:
            waterSurface->addGaussian(ALPHA, 0.25f*xSize, 0.25f*ySize, SIGMA_X, SIGMA_Y);
            break;
        default:
            fprintf(stderr, "Unknown interaction type: %i\n", interactionType);
            glfwTerminate();
            exit(-1);
    }
}

void Scene::advanceSimulation(float deltaT) {
    //water surface.
    waterSurface->advanceSimulation(deltaT);

    //objects.
    BoundingBox simulationBounds = bounds->getBoundingBox();
    for (int n = 0; n < objects.size(); n++) {
        ObjectInterface* object = objects[n];

        //advance position to next time step.
        vec3 position = object->getPosition();
        vec3 velocity = object->getVelocity();
        position += velocity * deltaT;
        object->setPosition(position);

        //update velocity.
        //object bounces elastically at simulation boundaries.
        BoundingBox objectBounds = object->getBoundingBox();
        bool zBounce = false;
        if (objectBounds.getMinX() <= simulationBounds.getMinX() && velocity[0] < 0) {
            velocity[0] *= -1;
        }
        if (objectBounds.getMaxX() >= simulationBounds.getMaxX() && velocity[0] > 0) {
            velocity[0] *= -1;
        }
        if (objectBounds.getMinY() <= simulationBounds.getMinY() && velocity[1] < 0) {
            velocity[1] *= -1;
        }
        if (objectBounds.getMaxY() >= simulationBounds.getMaxY() && velocity[1] > 0) {
            velocity[1] *= -1;
        }
        if (objectBounds.getMinZ() <= simulationBounds.getMinZ() && velocity[2] < 0) {
            velocity[2] *= -1;
            zBounce = true;
        }
        if (objectBounds.getMaxZ() >= simulationBounds.getMaxZ() && velocity[2] > 0) {
            velocity[2] *= -1;
            zBounce = true;
        }

        //apply gravity in negative z direction.
        //To make sure that energy is conserved, do not apply gravity during a bounce against the ground or ceiling.
        if (!zBounce) {//if object is not bouncing against the ground or ceiling at the moment.
            velocity[2] += - G * deltaT;
        }

        //apply forces from water surface on object.
        int vertexIndex = waterSurface->getIndexOfClosestVertex(position[0], position[1]);
        if (vertexIndex != -1) {//if object is above or below water surface.
            float waterSurfaceHeight = waterSurface->getSurfaceHeight(vertexIndex);
            if (objectBounds.getMinZ() <= waterSurfaceHeight) {//if object is floating or submersed.
                vec3 force = vec3(0);

                //horizontal force proportional and opposite to gradient of water surface.
                vec2 waterSurfaceGradient = waterSurface->getSurfaceGradient(vertexIndex);
                const float gradientCouplingConstant = 0.1f;//arbitrary coupling constant in kg*m/s2.
                force[0] = - gradientCouplingConstant * waterSurfaceGradient[0];
                force[1] = - gradientCouplingConstant * waterSurfaceGradient[1];

                //buoyancy.
                float displacedVolume = object->getVolumeBelowZ(waterSurfaceHeight);
                force[2] = displacedVolume * DENSITY_OF_WATER * G;

                //apply force.
                velocity += (force / object->getMass()) * deltaT;

                //friction due to moving through water.
                velocity[0] *= 0.99f;//arbitrary value.
                velocity[1] *= 0.99f;//arbitrary value.
                velocity[2] *= 0.5f;//arbitrary value.
            }
        }

        object->setVelocity(velocity);
    }
}
