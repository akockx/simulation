/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#ifndef INCLUDED__BOUNDINGBOX_H
#define INCLUDED__BOUNDINGBOX_H

/**
 * Bounding box.
 */
class BoundingBox {
    private:
        float xMin;//in m.
        float xMax;//in m.
        float yMin;//in m.
        float yMax;//in m.
        float zMin;//in m.
        float zMax;//in m.

    public:
        /**
         * Creates a bounding box with the given bounds (in m).
         */
        BoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

        /**
         * Getters.
         */
        float getMinX();
        float getMaxX();
        float getMinY();
        float getMaxY();
        float getMinZ();
        float getMaxZ();
};

#endif
