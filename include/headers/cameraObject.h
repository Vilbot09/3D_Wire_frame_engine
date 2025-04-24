#pragma once

#include "headers/mesh.h"

class CameraObject {
public:
    Vec3D pos;
    Vec3D lookDir = Vec3D(0, 0, 1);
    float yaw = 0.0f;

    CameraObject() {}
};