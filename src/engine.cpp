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
    Tri3D projected, xRotated, xyRotated, zyxRotated, transformed;
    
    transformed = tri;
    updateMatricies(obj);

    transformed *= xRotMat;
    transformed *= yRotMat;
    transformed *= zRotMat;
    transformed *= translationOffsetMatrix;

    /*
    // X Rotation
    multiplyMatrix4x4(xRotMat, tri.vecs[0], xRotated.vecs[0]);
    multiplyMatrix4x4(xRotMat, tri.vecs[1], xRotated.vecs[1]);
    multiplyMatrix4x4(xRotMat, tri.vecs[2], xRotated.vecs[2]);

    // Y Rotation
    multiplyMatrix4x4(yRotMat, xRotated.vecs[0], yRotated.vecs[0]);
    multiplyMatrix4x4(yRotMat, xRotated.vecs[1], yRotated.vecs[1]);
    multiplyMatrix4x4(yRotMat, xRotated.vecs[2], yRotated.vecs[2]);

    // Z Rotated
    multiplyMatrix4x4(zRotMat, yRotated.vecs[0], zRotated.vecs[0]);
    multiplyMatrix4x4(zRotMat, yRotated.vecs[1], zRotated.vecs[1]);
    multiplyMatrix4x4(zRotMat, yRotated.vecs[2], zRotated.vecs[2]);

    // Translate
    
    multiplyMatrix4x4(translationOffsetMatrix, zRotated.vecs[0], translated.vecs[0]);
    multiplyMatrix4x4(translationOffsetMatrix, zRotated.vecs[1], translated.vecs[1]);
    multiplyMatrix4x4(translationOffsetMatrix, zRotated.vecs[2], translated.vecs[2]);
    */

    Vec3D towardPlayer;
    towardPlayer.x = 0;
    towardPlayer.y = 0;
    towardPlayer.z = -1;

    Vec3D normal;
    calculateNormalVector(transformed, normal); 

    // Dot Product
    float dot = normal.x*towardPlayer.x+normal.y*towardPlayer.y+normal.z*towardPlayer.z;

    dot = 1;
    if (dot > 0) {
        // Projection
        multiplyMatrix4x4(matrix, transformed.vecs[0], projected.vecs[0]);
        multiplyMatrix4x4(matrix, transformed.vecs[1], projected.vecs[1]);
        multiplyMatrix4x4(matrix, transformed.vecs[2], projected.vecs[2]);
        

        // Scale into view for perspective
        
        projected.vecs[0].x += 1.0f; projected.vecs[0].y += 1.0f;
        projected.vecs[1].x += 1.0f; projected.vecs[1].y += 1.0f;
        projected.vecs[2].x += 1.0f; projected.vecs[2].y += 1.0f;

        projected.vecs[0].x *= width * 0.5f; projected.vecs[0].y *= height * 0.5f;
        projected.vecs[1].x *= width * 0.5f; projected.vecs[1].y *= height * 0.5f;
        projected.vecs[2].x *= width * 0.5f; projected.vecs[2].y *= height * 0.5f;
        

        //Scale into view for simple projection
        /*
        projected.vecs[0].x *= height/10; projected.vecs[0].y *= height/10;
        projected.vecs[1].x *= height/10; projected.vecs[1].y *= height/10;
        projected.vecs[2].x *= height/10; projected.vecs[2].y *= height/10;

        projected.vecs[0].x += width/2; projected.vecs[0].y += height/2;
        projected.vecs[1].x += width/2; projected.vecs[1].y += height/2;
        projected.vecs[2].x += width/2; projected.vecs[2].y += height/2;
        */
        

        drawTriangle2D(
            projected.vecs[0].x, projected.vecs[0].y,
            projected.vecs[1].x, projected.vecs[1].y,
            projected.vecs[2].x, projected.vecs[2].y
        );
    }
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

void Engine::drawMesh(Mat4x4 matrix, SceneObject obj) {
    for (const auto& tri : obj.mesh.tris) {
        drawTriangle3D(matrix, tri, obj);
    }
}

void Engine::calculateNormalVector(Tri3D tri, Vec3D &out) {
    Vec3D U, V;

    // Subtract Vectors
    U.x = tri.vecs[1].x - tri.vecs[0].x;
    U.y = tri.vecs[1].y - tri.vecs[0].y;
    U.z = tri.vecs[1].z - tri.vecs[0].z;

    V.x = tri.vecs[2].x - tri.vecs[0].x;
    V.y = tri.vecs[2].y - tri.vecs[0].y;
    V.z = tri.vecs[2].z - tri.vecs[0].z;

    // Cross Product
    out.x = U.y * V.z - U.z * V.y;
    out.y = U.z * V.x - U.x * V.y;
    out.z = U.x * V.y - U.y * V.x;
}

void Engine::multiplyMatrix4x4(Mat4x4 matrix, Vec3D in, Vec3D &out) {
    out.x = matrix.mat[0][0] * in.x + matrix.mat[1][0] * in.y + matrix.mat[2][0] * in.z + matrix.mat[3][0];
    out.y = matrix.mat[0][1] * in.x + matrix.mat[1][1] * in.y + matrix.mat[2][1] * in.z + matrix.mat[3][1];
    out.z = matrix.mat[0][2] * in.x + matrix.mat[1][2] * in.y + matrix.mat[2][2] * in.z + matrix.mat[3][2];
    float w = matrix.mat[0][3] * in.x + matrix.mat[1][3] * in.y + matrix.mat[2][3] * in.z + matrix.mat[3][3];

    if (w != 0.0f) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
}
