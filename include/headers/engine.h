#pragma once

#include "headers/mesh.h"
#include "raylib/raylib.h"
#include "headers/sceneObject.h"
#include "headers/cameraObject.h"
#include <cmath>

class Engine {
public:
    Vec3D vUp;
    Vec3D vTarget;

    CameraObject camera;

    Engine();
    
    void Render_Triangle3D(Tri3D, SceneObject);
    void Render_Triangle2D(int, int, int, int, int, int);
    void Render_SceneObject(SceneObject);

    Mat4x4 Matrix_RotationX(float);
    Mat4x4 Matrix_RotationY(float);
    Mat4x4 Matrix_RotationZ(float);
    Mat4x4 Matrix_Translation(float, float, float);
    Mat4x4 Matrix_Projection(CameraObject);
    Mat4x4 Matrix_PointAt(Vec3D, Vec3D, Vec3D);
    Mat4x4 Matrix_QuickInvert(Mat4x4);
};