#pragma once

#include "headers/mesh.h"
#include <cmath>

class CameraObject {
public:
    Vec3D pos;
    Vec3D lookDir;
    float yaw;

    int width;
    int height;

    float fAspectRatio;
    float fFov;
    float fFovRad;
    float fNear;
    float fFar;

    CameraObject() {
        yaw = 0.0f;
        lookDir = Vec3D(0, 0, 1);

        width = 1000;
        height = 700;

        fAspectRatio = (float)height/(float)width;
        fFov = 70.0f;
        fFovRad = 1.0f / std::tan(fFov * 0.5f / 180.0f * 3.141592f);
        fNear = 0.1f;
        fFar = 1000.0f;
    }
};