#include "relay/rle.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

rle::appState state;

// *******
// Setting up engine
// *******

void rle::Init() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Relay Engine", 1000, 700, SDL_WINDOW_HIGH_PIXEL_DENSITY);
    renderer = SDL_CreateRenderer(window, nullptr);

    // Populate appState
    state.window = window;
    state.renderer = renderer;
}

void rle::Run() {
    // Calling the start function that the user defines
    UserStart(state);

    bool running = true;
    SDL_Event e;

    // Main loop
    while (running) {

        // Checking for inputs
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) { // When player hits the cross
                running = false; // break out of the main loop
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Calling the update function that the user defines
        UserUpdate(state);

        // Draws the frame
        SDL_RenderPresent(renderer);
    }

    // Destroying everything before the program stops
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}



// *******
// Matricies (defining functions that will return a matrix based on some parameters)
// *******

rle::matrix4x4 rle::MatrixRotationX(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = 1;
    matrix.m[1][1] = SDL_cosf(rad);
    matrix.m[2][2] = SDL_cosf(rad);
    matrix.m[2][1] = SDL_sinf(rad);
    matrix.m[1][2] = -SDL_sinf(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationY(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = SDL_cosf(rad);
    matrix.m[1][1] = 1;
    matrix.m[2][2] = SDL_cosf(rad);
    matrix.m[0][2] = SDL_sinf(rad);
    matrix.m[2][0] = -SDL_sinf(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationZ(float rad) {
    rle::matrix4x4 matrix;
    matrix.m[0][0] = SDL_cosf(rad);
    matrix.m[1][1] = SDL_cosf(rad);
    matrix.m[2][2] = 1;
    matrix.m[1][0] = SDL_sinf(rad);
    matrix.m[0][1] = -SDL_sinf(rad);
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
    float ratio = camera.far / (camera.far - camera.near);
    rle::matrix4x4 matrix;
    matrix.m[0][0] = camera.aspectRatio * camera.fovRad;
    matrix.m[1][1] = camera.fovRad;
    matrix.m[2][2] = ratio;
    matrix.m[3][2] = -camera.near * ratio;
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}




// ******
// Rendering
// ******

void rle::RenderMesh3D(camera3d camera, mesh3d mesh) {
    // This a loop that will loop through all the triangles in mesh.t
    for (auto tri : mesh.t) {  
        RenderTriangle3D(camera, tri);
    }
}

void rle::RenderTriangle3D(camera3d camera, triangle3d triangle) {
    rle::triangle3d newTri = triangle;

    // Matrix multiplication goes here

    // Render the output triangle
    RenderTriangle2D(
        { newTri.p[0].x, newTri.p[0].y },
        { newTri.p[1].x, newTri.p[1].y },
        { newTri.p[2].x, newTri.p[2].y }
    );
}

void rle::RenderTriangle2D(vector2d v1, vector2d v2, vector2d v3) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
    SDL_RenderLine(renderer, (int)v2.x, (int)v2.y, (int)v3.x, (int)v3.y);
    SDL_RenderLine(renderer, (int)v3.x, (int)v3.y, (int)v1.x, (int)v1.y);
}

// *******
// Maths between different types
// *******