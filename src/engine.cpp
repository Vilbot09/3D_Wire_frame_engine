#include "headers/engine.h"
#include "raylib/raylib.h"
#include "headers/presets.h"
#include <iostream>

Engine::Engine() {
    InitWindow(WIDTH, HEIGHT, "3D Engine");
    SetTargetFPS(60);

    testCube.tris = {
        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST                                                      
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH                                                     
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST                                                      
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP                                                       
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM                                                    
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    };

    simpleProjection.mat[0][0] = 1;
    simpleProjection.mat[1][1] = 1;
    simpleProjection.mat[3][3] = 1;

    while (!WindowShouldClose())
    {
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
    tri.vecs[0].x *= 100; tri.vecs[0].y *= 100;
    tri.vecs[1].x *= 100; tri.vecs[1].y *= 100;
    tri.vecs[2].x *= 100; tri.vecs[2].y *= 100;

    tri.vecs[0].x += WIDTH/2; tri.vecs[0].y += HEIGHT/2;
    tri.vecs[1].x += WIDTH/2; tri.vecs[1].y += HEIGHT/2;
    tri.vecs[2].x += WIDTH/2; tri.vecs[2].y += HEIGHT/2;

    drawTriangle2D(
        tri.vecs[0].x, tri.vecs[0].y,
        tri.vecs[1].x, tri.vecs[1].y,
        tri.vecs[2].x, tri.vecs[2].y
    );
}

void Engine::drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawCircle(x1, y1, 4, BLUE);
    DrawCircle(x2, y2, 4, BLUE);
    DrawCircle(x3, y3, 4, BLUE);

    DrawLine(x1, y1, x2, y2, BLACK);
    DrawLine(x2, y2, x3, y3, BLACK);
    DrawLine(x3, y3, x1, y1, BLACK);
}

void Engine::drawMeshes() {
    std::cout << testCube.tris[0].vecs[2].y;

    drawTriangle3D(simpleProjection, testCube.tris[0]);
};

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