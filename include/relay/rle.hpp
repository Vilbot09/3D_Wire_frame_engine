#pragma once

#include <SDL3/SDL.h>
#include <vector> // For triangle3d
#include <string> 

namespace rle {

struct vector3d {
    float x, y, z, w;
};

struct vector2d {
    float x, y;
};

struct triangle3d {
    vector3d p[3];
};

struct mesh3d {
    std::vector<triangle3d> t;
};

struct matrix4x4 {
    float m[4][4] = { 0.0f };
};

struct appState {
    SDL_Renderer* renderer;
    SDL_Window* window;
};

struct camera3d {
    float fov = 90.0f;
    float fovRad = 1.0f / SDL_tanf(fov * 0.5f / 180.0f * SDL_PI_F);
    float near = 0.1f;
    float far = 1000.0f;
    float width = 1000.0f;
    float height = 700.0f;
    float aspectRatio = (int)height / (int)width;
    vector3d pos;
};

struct object3d {
    mesh3d mesh;
    vector3d pos;
};

matrix4x4 MatrixRotationX(float);
matrix4x4 MatrixRotationY(float);
matrix4x4 MatrixRotationZ(float);
matrix4x4 MatrixTranslation(float, float, float);
matrix4x4 MatrixProjection(camera3d);
matrix4x4 MatrixPointAt(vector3d, vector3d, vector3d);
matrix4x4 MatrixQuickInvert(matrix4x4);

void UserStart(appState);
void UserUpdate(appState);

void Init();
void Run();

void RenderMesh3D(camera3d, mesh3d);
void RenderTriangle3D(camera3d, triangle3d);
void RenderTriangle2D(vector2d, vector2d, vector2d);

}