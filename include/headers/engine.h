#pragma once

#include "raylib/raylib.h"
#include <vector>

struct Vec3D {
    float x, y, z, w;
};

struct Tri3D {
    Vec3D vecs[3];
};

struct Mesh3D {
    std::vector<Tri3D> tris;
};

struct Mat4x4 {
    float mat[4][4] = {0};
};

class Engine {
public:
    Mesh3D testCube;
    Mat4x4 simpleProjection;

    float FOV = 70;
    float FOVRad = FOV/180 * 3.141592;
    float nearClipPlane = 0.1f;
    float farClipPlane = 100.0f;

    
    Engine();
    
    void multiplyMatrix4x4(Mat4x4 &matrix, Vec3D &in, Vec3D &out);
    void drawTriangle3D(Tri3D tri);
    void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawMeshes();
};