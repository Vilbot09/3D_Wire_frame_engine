#pragma once

#include <SDL3/SDL.h>
#include <vector> // For triangle3d
#include <string> 
#include <iostream> // For debugging purposes

namespace rle {

// Declaring different structs
 
struct vector3d {
    float x, y, z, w;
};

struct vector2d {
    float x, y;
};

struct color {
    Uint8 r, g, b, a;  
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
    SDL_Event event;
    float deltaTime;
    float frameRate;
};

struct camera3d { // This will hold all the projection "settings", it will also have a position in the world
    int width = 1000.0f;
    int height = 700.0f;
    float fov = 90.0f;
    float near = 0.1f;
    float far = 1000.0f;
    float aspectRatio = (float)height / (float)width;

    vector3d pos;
    vector3d lookDir;
    float yaw;
    float pitch;

    void GetNewWindowSize(SDL_Window* window) {
        SDL_GetWindowSizeInPixels(window, &width, &height);
        aspectRatio = (float)height / (float)width;
    }
};

struct object3d {
    mesh3d mesh;
    vector3d pos;
    vector3d rot;
};

// Matrix functions
matrix4x4 MatrixRotationX(float);
matrix4x4 MatrixRotationY(float);
matrix4x4 MatrixRotationZ(float);
matrix4x4 MatrixTranslation(float, float, float);
matrix4x4 MatrixProjection(camera3d);
matrix4x4 MatrixPointAt(vector3d, vector3d, vector3d);
matrix4x4 MatrixQuickInvert(matrix4x4);

// Maths functions
vector3d AddVectors(vector3d, vector3d);
vector3d AddFloatToVector(vector3d, float);
vector3d MultiplyVectorByMatrix(vector3d, matrix4x4);
vector3d MultiplyVectorByFloat(vector3d, float);
vector3d MultiplyMatricies(matrix4x4, matrix4x4);

// Mesh functions
mesh3d LoadMeshFromObjectFile(std::string);

// Functions that the user will define
void UserStart(const appState&);
void UserUpdate(const appState&);
void UserInput(const appState&);

// Functions that will initualize and start SDL
void Init();
void Run();

// Function for rendering
void SetRenderColor(color);
void RenderObject3D(camera3d, object3d);
void RenderMesh3D(camera3d, object3d, mesh3d);
void RenderTriangle3D(camera3d, object3d, triangle3d);
void RenderTriangle2D(vector2d, vector2d, vector2d);

}