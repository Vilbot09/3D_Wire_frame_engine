#include "relay/rle.hpp"

#include <fstream> // For reading files
#include <strstream> // For reading files


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

            state.event = e;
            UserInput(state);
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


// ******
// Meshes
// ******

rle::mesh3d rle::LoadMeshFromObjectFile(std::string filename) {
    mesh3d newMesh;

    std::ifstream file(filename);
    if (!file.is_open()) return newMesh;

    std::vector<vector3d> verts;

    while (!file.eof()) {  // eof for "end of file"
        char line[128];
        file.getline(line, 128);

        std::strstream stream;
        stream << line;

        char junk;

        if (line[0] == 'v') { // We are reading a vertex
            vector3d vertex;
            stream >> junk >> vertex.x >> vertex.y >> vertex.z;
            verts.push_back(vertex);
        }

        if (line[0] == 'f') {
            int face[3];
            stream >> junk >> face[0] >> face[1] >> face[2];

            newMesh.t.push_back({
                verts[face[0]-1],
                verts[face[1]-1],
                verts[face[2]-1]
            });
        }

    }

    return newMesh;
}

// *******
// Matricies (defining functions that will return a matrix based on some parameters)
// *******

rle::matrix4x4 rle::MatrixRotationX(float rad) {
    matrix4x4 matrix;
    matrix.m[0][0] = 1;
    matrix.m[1][1] = SDL_cosf(rad);
    matrix.m[2][2] = SDL_cosf(rad);
    matrix.m[2][1] = SDL_sinf(rad);
    matrix.m[1][2] = -SDL_sinf(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationY(float rad) {
    matrix4x4 matrix;
    matrix.m[0][0] = SDL_cosf(rad);
    matrix.m[1][1] = 1;
    matrix.m[2][2] = SDL_cosf(rad);
    matrix.m[0][2] = SDL_sinf(rad);
    matrix.m[2][0] = -SDL_sinf(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixRotationZ(float rad) {
    matrix4x4 matrix;
    matrix.m[0][0] = SDL_cosf(rad);
    matrix.m[1][1] = SDL_cosf(rad);
    matrix.m[2][2] = 1;
    matrix.m[1][0] = SDL_sinf(rad);
    matrix.m[0][1] = -SDL_sinf(rad);
    return matrix;
}

rle::matrix4x4 rle::MatrixTranslation(float tX, float tY, float tZ) {
    matrix4x4 matrix;
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
    matrix4x4 matrix;
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

void rle::SetRenderColor(color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void rle::RenderMesh3D(camera3d camera, mesh3d mesh) {
    // This a loop that will loop through all the triangles in mesh.t
    for (auto tri : mesh.t) {  
        RenderTriangle3D(camera, tri);
    }
}

void rle::RenderTriangle3D(camera3d camera, triangle3d triangle) {
    triangle3d newTri = triangle;

    // Matrix multiplication goes here

    newTri.p[0] = MultiplyVectorByMatrix(newTri.p[0], MatrixRotationX(0.0f));
    newTri.p[1] = MultiplyVectorByMatrix(newTri.p[1], MatrixRotationX(0.0f));
    newTri.p[2] = MultiplyVectorByMatrix(newTri.p[2], MatrixRotationX(0.0f));

    newTri.p[0] = MultiplyVectorByMatrix(newTri.p[0], MatrixRotationY(0.0f));
    newTri.p[1] = MultiplyVectorByMatrix(newTri.p[1], MatrixRotationY(0.0f));
    newTri.p[2] = MultiplyVectorByMatrix(newTri.p[2], MatrixRotationY(0.0f));

    newTri.p[0] = MultiplyVectorByMatrix(newTri.p[0], MatrixRotationZ(0.0f));
    newTri.p[1] = MultiplyVectorByMatrix(newTri.p[1], MatrixRotationZ(0.0f));
    newTri.p[2] = MultiplyVectorByMatrix(newTri.p[2], MatrixRotationZ(0.0f));

    // Translate
    newTri.p[0].z += 20.0f;
    newTri.p[1].z += 20.0f;
    newTri.p[2].z += 20.0f;

    newTri.p[0] = MultiplyVectorByMatrix(newTri.p[0], MatrixProjection(camera));
    newTri.p[1] = MultiplyVectorByMatrix(newTri.p[1], MatrixProjection(camera));
    newTri.p[2] = MultiplyVectorByMatrix(newTri.p[2], MatrixProjection(camera));

    // Dividing by W
    newTri.p[0].x /= newTri.p[0].w; newTri.p[0].y /= newTri.p[0].w; newTri.p[0].z /= newTri.p[0].w;
    newTri.p[1].x /= newTri.p[1].w; newTri.p[1].y /= newTri.p[1].w; newTri.p[1].z /= newTri.p[1].w;
    newTri.p[2].x /= newTri.p[2].w; newTri.p[2].y /= newTri.p[2].w; newTri.p[2].z /= newTri.p[2].w;

    // Scale into view here
    newTri.p[0] = AddFloatToVector(newTri.p[0], 1.0f);
    newTri.p[1] = AddFloatToVector(newTri.p[1], 1.0f);
    newTri.p[2] = AddFloatToVector(newTri.p[2], 1.0f);
    
    newTri.p[0].x *= camera.width * 0.5f;  newTri.p[0].y *= camera.height * 0.5f;
    newTri.p[1].x *= camera.width * 0.5f;  newTri.p[1].y *= camera.height * 0.5f;
    newTri.p[2].x *= camera.width * 0.5f;  newTri.p[2].y *= camera.height * 0.5f;

    // Render the output triangle
    RenderTriangle2D(
        { newTri.p[0].x, newTri.p[0].y },
        { newTri.p[1].x, newTri.p[1].y },
        { newTri.p[2].x, newTri.p[2].y }
    );
}

void rle::RenderTriangle2D(vector2d v1, vector2d v2, vector2d v3) {
    SDL_RenderLine(renderer, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y);
    SDL_RenderLine(renderer, (int)v2.x, (int)v2.y, (int)v3.x, (int)v3.y);
    SDL_RenderLine(renderer, (int)v3.x, (int)v3.y, (int)v1.x, (int)v1.y);                                    
}




// *******
// Maths between different types
// *******

rle::vector3d rle::AddFloatToVector(vector3d v, float f) {
    vector3d newV = v;
    newV.x = v.x + f;
    newV.y = v.y + f;
    newV.z = v.z + f;
    return newV;
}

rle::vector3d rle::MultiplyVectorByMatrix(vector3d v, matrix4x4 m) {
    vector3d newV = v;
    newV.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    newV.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    newV.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    newV.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
    return newV;
}

rle::vector3d rle::MultiplyVectorByFloat(vector3d v, float f) {
    vector3d newV = v;
    newV.x = v.x * f;
    newV.y = v.y * f;
    newV.z = v.z * f;
    return newV;
}