#include "headers/engine.h"
#include "raylib/raylib.h"
#include "headers/presets.h"
#include <iostream>

Engine::Engine() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 700, "3D Engine");
    SetTargetFPS(60);
    MaximizeWindow();

    testCube.tris = MeshPreset::cube;

    simpleProjection.mat[0][0] = 1;
    simpleProjection.mat[1][1] = 1;
    simpleProjection.mat[3][3] = 1;

    while (!WindowShouldClose())
    {
        width = GetScreenWidth();
        height = GetScreenHeight();
        aspectRatio = height/width;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawMeshes();
        EndDrawing();
    }

    CloseWindow();
}

void Engine::drawTriangle3D(Mat4x4 matrix, Tri3D tri) {
    Tri3D out;

    multiplyMatrix4x4(matrix, tri.vecs[0], out.vecs[0]);
    multiplyMatrix4x4(matrix, tri.vecs[1], out.vecs[1]);
    multiplyMatrix4x4(matrix, tri.vecs[2], out.vecs[2]);

    // Scale into view
    out.vecs[0].x *= height/2; out.vecs[0].y *= height/2;
    out.vecs[1].x *= height/2; out.vecs[1].y *= height/2;
    out.vecs[2].x *= height/2; out.vecs[2].y *= height/2;

    out.vecs[0].x += width/2; out.vecs[0].y += height/2;
    out.vecs[1].x += width/2; out.vecs[1].y += height/2;
    out.vecs[2].x += width/2; out.vecs[2].y += height/2;

    drawTriangle2D(
        out.vecs[0].x, out.vecs[0].y,
        out.vecs[1].x, out.vecs[1].y,
        out.vecs[2].x, out.vecs[2].y
    );
}

void Engine::drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawLine(x1, y1, x2, y2, BLACK);
    DrawLine(x2, y2, x3, y3, BLACK);
    DrawLine(x3, y3, x1, y1, BLACK);

    DrawCircle(x1, y1, 4, BLUE);
    DrawCircle(x2, y2, 4, BLUE);
    DrawCircle(x3, y3, 4, BLUE);
}

void Engine::drawMeshes() {
    drawMesh(simpleProjection, testCube);
};

void Engine::drawMesh(Mat4x4 matrix, Mesh3D mesh) {
    for (auto tri : mesh.tris) {
        drawTriangle3D(matrix, tri);
    }
}

void Engine::multiplyMatrix4x4(Mat4x4 matrix, Vec3D in, Vec3D &out) {
    out.x = matrix.mat[0][0] * in.x + matrix.mat[1][0] * in.y + matrix.mat[2][0] * in.z;
    out.y = matrix.mat[0][1] * in.x + matrix.mat[1][1] * in.y + matrix.mat[2][1] * in.z;
    out.z = matrix.mat[0][2] * in.x + matrix.mat[1][2] * in.y + matrix.mat[2][2] * in.z + matrix.mat[3][2];
    float w = matrix.mat[0][3] * in.x + matrix.mat[1][3] * in.y + matrix.mat[2][3] * in.z;

    //for (int i = 0; i < sizeof(matrix.mat)/sizeof(matrix.mat[0]); i++) {
      //  for (int r = 0; r < sizeof(matrix.mat[0])/sizeof(matrix.mat[0][0]); i++) {
          //  std::cout << i+r;
        //}
      //}
}