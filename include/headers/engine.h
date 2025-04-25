#pragma once

#include "headers/mesh.h"
#include "raylib/raylib.h"
#include "headers/sceneObject.h"
#include "headers/cameraObject.h"
#include <cmath>

class Engine {
public:
    Vec3D upVector;
    Vec3D targetVector;

    CameraObject camera;

    Engine();
    
    void Render_Triangle3D(Tri3D tri, SceneObject obj);
    void Render_Triangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void Render_SceneObject(SceneObject obj);

    Mat4x4 Matrix_RotationX(float);
    Mat4x4 Matrix_RotationY(float);
    Mat4x4 Matrix_RotationZ(float);
    Mat4x4 Matrix_Translation(float, float, float);
    Mat4x4 Matrix_Projection(CameraObject);
    Mat4x4 Matrix_PointAt(Vec3D, Vec3D, Vec3D);
    Mat4x4 Matrix_QuickInvert(Mat4x4);
};