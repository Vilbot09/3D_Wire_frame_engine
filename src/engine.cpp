#include "headers/sceneObject.h"
#include "headers/mesh.h"
#include "headers/engine.h"
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

    //testCube.LoadFromObjectFile("../meshes/ship.obj");
    //testCube.tris = MeshPreset::cube;

    //floor.LoadFromObjectFile("../meshes/plane.obj");

    simpleProjection.mat[0][0] = 1;
    simpleProjection.mat[1][1] = 1;
    simpleProjection.mat[2][2] = 1;
    simpleProjection.mat[3][3] = 1;
}

void Engine::updateMatricies(SceneObject obj) {

    targetVector = camera.pos + camera.lookDir;
    cameraMatrix = makePointAtMatrix(camera.pos, targetVector, upVector);
    viewMatrix = quickInvertMatrix(cameraMatrix);

    // Perspective
    q = farClipPlane/(farClipPlane-nearClipPlane);
    perspective.mat[0][0] = aspectRatio*FOVRad;
    perspective.mat[1][1] = FOVRad;
    perspective.mat[2][2] = q;
    perspective.mat[3][2] = -nearClipPlane*q;
    perspective.mat[2][3] = 1.0f;
    perspective.mat[3][3] = 0.0f;

    // Setup Rotation Matricies
    xRotMat.mat[0][0] = 1;
    xRotMat.mat[1][1] = std::cos(obj.rot.x);
    xRotMat.mat[2][2] = std::cos(obj.rot.x);
    xRotMat.mat[2][1] = std::sin(obj.rot.x);
    xRotMat.mat[1][2] = -std::sin(obj.rot.x);
    
    yRotMat.mat[0][0] = std::cos(obj.rot.y);
    yRotMat.mat[1][1] = 1;
    yRotMat.mat[2][2] = std::cos(obj.rot.y);
    yRotMat.mat[0][2] = std::sin(obj.rot.y);
    yRotMat.mat[2][0] = -std::sin(obj.rot.y);

    zRotMat.mat[0][0] = std::cos(obj.rot.z);
    zRotMat.mat[1][1] = std::cos(obj.rot.z);
    zRotMat.mat[2][2] = 1;
    zRotMat.mat[1][0] = std::sin(obj.rot.z);
    zRotMat.mat[0][1] = -std::sin(obj.rot.z);

    // Translation
    translationOffsetMatrix.mat[0][0] = 1.0f;
    translationOffsetMatrix.mat[3][0] = obj.pos.x;
    translationOffsetMatrix.mat[1][1] = 1.0f;
    translationOffsetMatrix.mat[3][1] = obj.pos.y;
    translationOffsetMatrix.mat[2][2] = 1.0f;
    translationOffsetMatrix.mat[3][2] = obj.pos.z;
    translationOffsetMatrix.mat[2][3] = 0.0f;
    translationOffsetMatrix.mat[3][3] = 1.0f;
}

void Engine::drawTriangle3D(Mat4x4 matrix, Tri3D tri, SceneObject obj) {
    Tri3D projected, transformed;
    
    updateMatricies(obj);

    transformed = tri;
    transformed *= xRotMat;
    transformed *= yRotMat;
    transformed *= zRotMat;
    transformed *= translationOffsetMatrix;
    transformed *= viewMatrix;

    projected = transformed;
    projected *= matrix; 

    // Divide by W
    projected.vecs[0] /= projected.vecs[0].w;
    projected.vecs[1] /= projected.vecs[1].w;
    projected.vecs[2] /= projected.vecs[2].w;

    // Scale into view
    projected += 1.0f;
    projected.vecs[0].x *= width * 0.5f; projected.vecs[0].y *= height * 0.5f;
    projected.vecs[1].x *= width * 0.5f; projected.vecs[1].y *= height * 0.5f;
    projected.vecs[2].x *= width * 0.5f; projected.vecs[2].y *= height * 0.5f;

    drawTriangle2D(
        projected.vecs[0].x, projected.vecs[0].y,
        projected.vecs[1].x, projected.vecs[1].y,
        projected.vecs[2].x, projected.vecs[2].y
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

void Engine::drawObject(Mat4x4 matrix, SceneObject obj) {
    for (const auto& tri : obj.mesh.tris) {
        drawTriangle3D(matrix, tri, obj);
    }
}

Mat4x4 Engine::makePointAtMatrix(Vec3D& pos, Vec3D& target, Vec3D& up) {
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

Mat4x4 Engine::quickInvertMatrix(Mat4x4 m) {
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