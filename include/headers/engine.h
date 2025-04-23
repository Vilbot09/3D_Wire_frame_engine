#pragma once

#include "headers/mesh.h"
#include "raylib/raylib.h"
#include "headers/sceneObject.h"
#include <cmath>

class Engine {
public:
    Mesh3D testCube;
    Mesh3D floor;

    Mat4x4 simpleProjection;
    Mat4x4 perspective;

    // Rotation Matricies
    Mat4x4 xRotMat;
    Mat4x4 yRotMat;
    Mat4x4 zRotMat;

    // Translation Matrix
    Mat4x4 translationOffsetMatrix;


    int width = 1000;
    int height = 700;

    float aspectRatio = (float)height/(float)width;
    float FOV = 70.0f;
    float FOVRad = 1.0f / std::tan(FOV * 0.5f / 180.0f * 3.141592f);
    float nearClipPlane = 0.1f;
    float farClipPlane = 10000.0f;
    float q = farClipPlane/(farClipPlane-nearClipPlane);


    Engine();
    
    void multiplyMatrix4x4(Mat4x4 matrix, Vec3D in, Vec3D &out);
    void calculateNormalVector(Tri3D tri, Vec3D &out);
    void drawTriangle3D(Mat4x4 matrix, Tri3D tri, SceneObject obj);
    void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawMesh(Mat4x4 matrix, SceneObject obj);
    void updateMatricies(SceneObject obj);
};