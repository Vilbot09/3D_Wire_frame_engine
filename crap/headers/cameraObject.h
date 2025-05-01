#pragma once

#include "raylib/raylib.h"
#include "headers/mesh.h"
#include <cmath>

class CameraObject {
public:
    Vec3D vPos;
    Vec3D vLookDir;

    float fYaw;
    float fPitch;

    float fSpeed;
    float fPitchSpeed;
    float fYawSpeed;

    float fAspectRatio;
    float fFov;
    float fFovRad;
    float fNear;
    float fFar;

    CameraObject() {
        fSpeed = 0.3f;
        fYawSpeed = 0.04f;
        fPitchSpeed = 0.03f;

        fYaw = 0.0f;
        fPitch = 0.0f;
        vLookDir = Vec3D(0, 0, 1);

        fAspectRatio = (float)Get_Height()/(float)Get_Width();
        fFov = 70.0f;
        fFovRad = 1.0f / std::tan(fFov * 0.5f / 180.0f * 3.141592f);
        fNear = 0.1f;
        fFar = 1000.0f;
    }

    int Get_Width() {return GetScreenWidth();}
    int Get_Height() {return GetScreenHeight();}
};