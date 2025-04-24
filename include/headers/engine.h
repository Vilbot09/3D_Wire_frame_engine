#pragma once

#include "headers/mesh.h"
#include "raylib/raylib.h"
#include "headers/sceneObject.h"
#include "headers/cameraObject.h"
#include <cmath>

class Engine {

public:
    CameraObject camera;

    Engine();
    
    void drawTriangle3D(Tri3D tri, SceneObject obj);
    void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawObject(SceneObject obj);

    Mat4x4 Matrix_RotationX(float);
    Mat4x4 Matrix_RotationY(float);
    Mat4x4 Matrix_RotationZ(float);
    Mat4x4 Matrix_Translation(float, float, float);
    Mat4x4 Matrix_Projection(CameraObject);
    Mat4x4 Matrix_PointAt(Vec3D, Vec3D, Vec3D);
    Mat4x4 Matrix_QuickInvert(Mat4x4);
};