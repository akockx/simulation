/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/BoundingBox.h"

BoundingBox::BoundingBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    this->zMin = zMin;
    this->zMax = zMax;
}

float BoundingBox::getMinX() {
    return xMin;
}

float BoundingBox::getMaxX() {
    return xMax;
}

float BoundingBox::getMinY() {
    return yMin;
}

float BoundingBox::getMaxY() {
    return yMax;
}

float BoundingBox::getMinZ() {
    return zMin;
}

float BoundingBox::getMaxZ() {
    return zMax;
}
