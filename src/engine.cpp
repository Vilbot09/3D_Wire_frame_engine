#include "headers/sceneObject.h"
#include "headers/mesh.h"
#include "headers/engine.h"
#include "headers/cameraObject.h"
#include "raylib/raylib.h"
#include <iostream>
#include <cmath>

Engine::Engine() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 700, "3D Engine");
    SetTargetFPS(60);
    //ToggleFullscreen();
    //MaximizeWindow();

    Image icon;
    icon = LoadImage("../images/engine.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
}

void Engine::drawTriangle3D(Tri3D tri, SceneObject obj) {
    Vec3D upVector = Vec3D(0, 1, 0);
    Vec3D targetVector = Vec3D(0, 0, 1);
    Mat4x4 camRotMat = Matrix_RotationY(camera.yaw);
    camera.lookDir = targetVector * camRotMat;
    targetVector = camera.pos + camera.lookDir;

    Tri3D newTri;
    newTri = tri;

    newTri *= Matrix_RotationX(obj.rot.x);
    newTri *= Matrix_RotationY(obj.rot.y);
    newTri *= Matrix_RotationZ(obj.rot.z);
    newTri *= Matrix_Translation(obj.pos.x, obj.pos.y, obj.pos.z);
    newTri *= Matrix_QuickInvert(Matrix_PointAt(camera.pos, targetVector, upVector));
    newTri *= Matrix_Projection(camera); 

    // Divide by W
    newTri.vecs[0] /= newTri.vecs[0].w;
    newTri.vecs[1] /= newTri.vecs[1].w;
    newTri.vecs[2] /= newTri.vecs[2].w;

    // Scale into view
    newTri += 1.0f;
    newTri.vecs[0].x *= camera.width * 0.5f; newTri.vecs[0].y *= camera.height * 0.5f;
    newTri.vecs[1].x *= camera.width * 0.5f; newTri.vecs[1].y *= camera.height * 0.5f;
    newTri.vecs[2].x *= camera.width * 0.5f; newTri.vecs[2].y *= camera.height * 0.5f;

    drawTriangle2D(
        newTri.vecs[0].x, newTri.vecs[0].y,
        newTri.vecs[1].x, newTri.vecs[1].y,
        newTri.vecs[2].x, newTri.vecs[2].y
    );
    
}

void Engine::drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawLine(x1, y1, x2, y2, WHITE);
    DrawLine(x2, y2, x3, y3, WHITE);
    DrawLine(x3, y3, x1, y1, WHITE);

    /*
    Vector2 v1 = {(float)x1, (float)y1};
    Vector2 v2 = {(float)x2, (float)y2};
    Vector2 v3 = {(float)x3, (float)y3};

    DrawTriangle(v1, v2, v3, RAYWHITE); // Color should be bg-color
    */

    //DrawCircle(x1, y1, 4, BLUE);
    //DrawCircle(x2, y2, 4, BLUE);
    //DrawCircle(x3, y3, 4, BLUE);
}

void Engine::drawObject(SceneObject obj) {
    for (const auto& tri : obj.mesh.tris) {
        drawTriangle3D(tri, obj);
    }
}

Mat4x4 Engine::Matrix_RotationX(float rad) {
    Mat4x4 matrix;
    matrix.mat[0][0] = 1;
    matrix.mat[1][1] = std::cos(rad);
    matrix.mat[2][2] = std::cos(rad);
    matrix.mat[2][1] = std::sin(rad);
    matrix.mat[1][2] = -std::sin(rad);
    return matrix;
}

Mat4x4 Engine::Matrix_RotationY(float rad) {
    Mat4x4 matrix;
    matrix.mat[0][0] = std::cos(rad);
    matrix.mat[1][1] = 1;
    matrix.mat[2][2] = std::cos(rad);
    matrix.mat[0][2] = std::sin(rad);
    matrix.mat[2][0] = -std::sin(rad);
    return matrix;
}

Mat4x4 Engine::Matrix_RotationZ(float rad) {
    Mat4x4 matrix;
    matrix.mat[0][0] = std::cos(rad);
    matrix.mat[1][1] = std::cos(rad);
    matrix.mat[2][2] = 1;
    matrix.mat[1][0] = std::sin(rad);
    matrix.mat[0][1] = -std::sin(rad);
    return matrix;
}

Mat4x4 Engine::Matrix_Translation(float tX, float tY, float tZ) {
    Mat4x4 matrix;
    matrix.mat[0][0] = 1.0f;
    matrix.mat[1][1] = 1.0f;
    matrix.mat[2][2] = 1.0f;
    matrix.mat[3][3] = 1.0f;
    matrix.mat[3][0] = tX;
    matrix.mat[3][1] = tY;
    matrix.mat[3][2] = tZ;
    return matrix;
}

Mat4x4 Engine::Matrix_Projection(CameraObject camera) {
    float fRatio = camera.fFar / (camera.fFar - camera.fNear);

    Mat4x4 matrix;
    matrix.mat[0][0] = camera.fAspectRatio * camera.fFovRad;
    matrix.mat[1][1] = camera.fFovRad;
    matrix.mat[2][2] = fRatio;
    matrix.mat[3][2] = -camera.fNear * fRatio;
    matrix.mat[2][3] = 1.0f;
    matrix.mat[3][3] = 0.0f;
    return matrix;
}

Mat4x4 Engine::Matrix_PointAt(Vec3D pos, Vec3D target, Vec3D up) {
    Vec3D newForward = target - pos;
    newForward.normalize();

    float dotProduct = up.dot(newForward);
    Vec3D a = newForward * dotProduct;
    Vec3D newUp = up - a;
    newUp.normalize();
    //std::cout << newUp.y << std::endl;
    //std::cout.flush();
    
    // std cout med flush förstör framRaten ...
    Vec3D newRight = newUp.cross(newForward);

    Mat4x4 newMat;
    newMat.mat[0][0] = newRight.x;       newMat.mat[0][1] = newRight.y;       newMat.mat[0][2] = newRight.z;      newMat.mat[0][3] = 0.0f;
    newMat.mat[1][0] = newUp.x;          newMat.mat[1][1] = newUp.y;          newMat.mat[1][2] = newUp.z;         newMat.mat[1][3] = 0.0f;
    newMat.mat[2][0] = newForward.x;     newMat.mat[2][1] = newForward.y;     newMat.mat[2][2] = newForward.z;    newMat.mat[2][3] = 0.0f;
    newMat.mat[3][0] = pos.x;            newMat.mat[3][1] = pos.y;            newMat.mat[3][2] = pos.z;           newMat.mat[3][3] = 1.0f;
    return newMat;
}

Mat4x4 Engine::Matrix_QuickInvert(Mat4x4 m) {
    Mat4x4 matrix;
    matrix.mat[0][0] = m.mat[0][0]; matrix.mat[0][1] = m.mat[1][0]; matrix.mat[0][2] = m.mat[2][0]; matrix.mat[0][3] = 0.0f;
    matrix.mat[1][0] = m.mat[0][1]; matrix.mat[1][1] = m.mat[1][1]; matrix.mat[1][2] = m.mat[2][1]; matrix.mat[1][3] = 0.0f;
    matrix.mat[2][0] = m.mat[0][2]; matrix.mat[2][1] = m.mat[1][2]; matrix.mat[2][2] = m.mat[2][2]; matrix.mat[2][3] = 0.0f;
    matrix.mat[3][0] = -(m.mat[3][0] * matrix.mat[0][0] + m.mat[3][1] * matrix.mat[1][0] + m.mat[3][2] * matrix.mat[2][0]);
    matrix.mat[3][1] = -(m.mat[3][0] * matrix.mat[0][1] + m.mat[3][1] * matrix.mat[1][1] + m.mat[3][2] * matrix.mat[2][1]);
    matrix.mat[3][2] = -(m.mat[3][0] * matrix.mat[0][2] + m.mat[3][1] * matrix.mat[1][2] + m.mat[3][2] * matrix.mat[2][2]);
    matrix.mat[3][3] = 1.0f;
    return matrix;
}