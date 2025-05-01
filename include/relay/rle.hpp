#pragma once

#include <SDL3/SDL.h>
#include <vector> // For triangle3d
#include <string> 

namespace rle {

// Declaring different structs
 
struct vector3d {
    float x, y, z, w;
};

struct vector2d {
    float x, y;
};

struct triangle3d {
    vector3d p[3]; // Array of three point
};

struct mesh3d {
    std::vector<triangle3d> t; // This is essentially a list with an undetermined amount of triangles
};

struct matrix4x4 {
    float m[4][4] = { 0.0f }; // [row][col] or [y][x], default value is 0
};

struct appState { // Will be given to the user in UserCreate and UserUpdate so they can use it
    SDL_Renderer* renderer;
    SDL_Window* window;
};

struct camera3d { // This will handle all the projection "setting", it will also have a position in the world
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

// Matrix functions
matrix4x4 MatrixRotationX(float);
matrix4x4 MatrixRotationY(float);
matrix4x4 MatrixRotationZ(float);
matrix4x4 MatrixTranslation(float, float, float);
matrix4x4 MatrixProjection(camera3d);
matrix4x4 MatrixPointAt(vector3d, vector3d, vector3d);
matrix4x4 MatrixQuickInvert(matrix4x4);

// Functions that the user will define
void UserStart(appState);
void UserUpdate(appState);

// Functions that will initualize and start SDL
void Init();
void Run();

// Function for rendering
void RenderMesh3D(camera3d, mesh3d);
void RenderTriangle3D(camera3d, triangle3d);
void RenderTriangle2D(vector2d, vector2d, vector2d);

}