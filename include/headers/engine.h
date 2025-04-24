#pragma once

#include "headers/mesh.h"
#include "raylib/raylib.h"
#include "headers/sceneObject.h"
#include "headers/cameraObject.h"
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

    CameraObject camera;
    Vec3D upVector = Vec3D(0, 1, 0);
    Vec3D targetVector = Vec3D(0, 0, 1);
    // Then multiply targetVector with fYaw
    Mat4x4 cameraMatrix = makePointAtMatrix(camera.pos, targetVector, upVector);
    Mat4x4 viewMatrix = quickInvertMatrix(cameraMatrix);


    int width = 1000;
    int height = 700;

    float aspectRatio = (float)height/(float)width;
    float FOV = 70.0f;
    float FOVRad = 1.0f / std::tan(FOV * 0.5f / 180.0f * 3.141592f);
    float nearClipPlane = 0.1f;
    float farClipPlane = 10000.0f;
    float q = farClipPlane/(farClipPlane-nearClipPlane);


    Engine();
    
    // void multiplyMatrix4x4(Mat4x4 matrix, Vec3D in, Vec3D &out);
    // void calculateNormalVector(Tri3D tri, Vec3D &out);
    void drawTriangle3D(Mat4x4 matrix, Tri3D tri, SceneObject obj);
    void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawObject(Mat4x4 matrix, SceneObject obj);
    void updateMatricies(SceneObject obj);

    Mat4x4 makePointAtMatrix(Vec3D& pos, Vec3D& target, Vec3D& up);
    Mat4x4 quickInvertMatrix(Mat4x4 m);
};