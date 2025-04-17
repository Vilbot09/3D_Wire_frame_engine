#include "headers/engine.h"
#include "raylib/raylib.h"
#include "headers/presets.h"
#include <iostream>

Engine::Engine() {
    InitWindow(800, 450, "raylib [core] example - basic window");

    testCube.tris = cubeTris;

    simpleProjection.mat[0][0] = 1;
    simpleProjection.mat[1][1] = 1;
    simpleProjection.mat[2][2] = 1;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawMeshes();
        EndDrawing();
    }

    CloseWindow();
}

void Engine::drawTriangle3D(Tri3D tri) {

}

void Engine::drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawLine(x1, y1, x2, y2, BLACK);
    DrawLine(x2, y2, x3, y3, BLACK);
    DrawLine(x3, y3, x1, y1, BLACK);
}

void Engine::drawMeshes() {
    drawTriangle2D(0, 0, 100, 100, 300, 200);

};

void Engine::multiplyMatrix4x4(Mat4x4 &matrix, Vec3D &in, Vec3D &out) {
    out.x = matrix.mat[0][0] * in.x + matrix.mat[1][0] * in.y + matrix.mat[2][0] * in.z;
    out.y = matrix.mat[0][1] * in.x + matrix.mat[1][1] * in.y + matrix.mat[2][1] * in.z;
    out.z = matrix.mat[0][2] * in.x + matrix.mat[1][2] * in.y + matrix.mat[2][2] * in.z;
    out.w;

    //for (int i = 0; i < sizeof(matrix.mat)/sizeof(matrix.mat[0]); i++) {
      //  for (int r = 0; r < sizeof(matrix.mat[0])/sizeof(matrix.mat[0][0]); i++) {
          //  std::cout << i+r;
        //}
      //}
}