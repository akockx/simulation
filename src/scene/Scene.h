/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/ModelUtils.h"
#include "model/SimulationBoundaries.h"
#include "model/ObjectInterface.h"
#include "model/WaterSurface.h"

#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

enum {
    ADD_WAVE_IN_SOUTH_WEST_CORNER_INTERACTION_TYPE,
    ADD_WAVE_IN_SOUTH_EAST_CORNER_INTERACTION_TYPE,
    ADD_WAVE_IN_NORTH_WEST_CORNER_INTERACTION_TYPE,
    ADD_WAVE_IN_NORTH_EAST_CORNER_INTERACTION_TYPE
};

/**
 * Stores and renders a collection of objects. Also controls user interaction and interactions between objects.
 */
class Scene {
    private:
        //objects.
        SimulationBoundaries* bounds;
        WaterSurface* waterSurface;
        vector<ObjectInterface*> objects;

        //camera.
        mat4 viewMatrix;

        //light source coordinates (x, y, z) in world space.
        float lightPositionInWorldSpace[3] = {10, -10, 10};
        //light source intensity per color component (r, g, b).
        float lightIntensity[3] = {300, 250, 200};
        //ambient light intensity per color component (r, g, b).
        float ambientLightIntensity[3] = {0.2f, 0.3f, 0.4f};

    public:
        Scene();

        /**
         * Performs the specified user interaction.
         */
        void interact(int interactionType);

        /**
         * Advances physics simulation of objects in this scene by the given deltaT (in seconds).
         */
        void advanceSimulation(float deltaT);

        /**
         * Renders all objects in this scene to the current OpenGL context.
         */
        void render(int width, int height);

        ~Scene();
};

#endif
