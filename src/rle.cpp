#include "relay/rle.hpp"
#include "raylib/raylib.h"
#include <cmath>

// *****
// Engine
// *****

rle::engine3d::engine3d() {
    InitWindow(500, 500, "Engine");
    SetTargetFPS(60);
}

void rle::engine3d::Start() {
    OnUserCreate();

    while (!WindowSouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        OnUserUpdate();
        EndDrawing();
    }

    CloseWindow();
}

// *******
// Matricies
// *******

rle::matrix4x4 rle::MatrixRotationX(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = 1;
    matrix.m[1][1] = std::cos(rad);
    matrix.m[2][2] = std::cos(rad);
    matrix.m[2][1] = std::sin(rad);
    matrix.m[1][2] = -std::sin(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationY(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = std::cos(rad);
    matrix.m[1][1] = 1;
    matrix.m[2][2] = std::cos(rad);
    matrix.m[0][2] = std::sin(rad);
    matrix.m[2][0] = -std::sin(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationZ(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = std::cos(rad);
    matrix.m[1][1] = std::cos(rad);
    matrix.m[2][2] = 1;
    matrix.m[1][0] = std::sin(rad);
    matrix.m[0][1] = -std::sin(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixTranslation(float tX, float tY, float tZ) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = tX;
    matrix.m[3][1] = tY;
    matrix.m[3][2] = tZ;
    return matrix;
}

rle::matrix4x4 rle::MatrixProjection(camera3d camera) {
    float fRatio = camera.fFar / (camera.fFar - camera.fNear);
    rle::matrix4x4 matrix;
    matrix.m[0][0] = camera.fAspectRatio * camera.fFovRad;
    matrix.m[1][1] = camera.fFovRad;
    matrix.m[2][2] = fRatio;
    matrix.m[3][2] = -camera.fNear * fRatio;
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}




// ******
// Rendering
// ******

void rle::RenderMesh3D(mesh3d mesh) {
    
}

void rle::RenderTriangle3D(camera3d camera, triangle3d triangle) {

}

void rle::RenderTriangle2D(vector2d v1, vector2d v2, vector2d v3) {
    DrawLine(v1.x, v1.y, v2.x, v2.y, BLACK);
    DrawLine(v2.x, v2.y, v3.x, v3.y, BLACK);
    DrawLine(v3.x, v3.y, v1.x, v1.y, BLACK);
}