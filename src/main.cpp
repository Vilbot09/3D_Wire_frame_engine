#include "relay/rle.hpp"

rle::vector3d v1 = { 0.0f, 0.0f, 20.0f, 1.0f }; // Last value should always be one
rle::vector3d v2 = { 100.0f, 50.0f, 18.0f, 1.0f };
rle::vector3d v3 = { 200.0f, 20.0f, 19.0f, 1.0f };

rle::triangle3d t1 = { 0.0f, 0.0f, 20.0f, 1.0f,     100.0f, 50.0f, 18.0f, 1.0f,     200.0f, 20.0f, 19.0f, 1.0f };
rle::camera3d camera;

void rle::UserStart(rle::appState appState) {

}

void rle::UserUpdate(rle::appState appState) {
    SDL_SetRenderDrawColor(appState.renderer, 0, 0, 0, 255);

    rle::RenderTriangle3D(camera, t1);

}


int main() {
    rle::Init();
    rle::Run();
    return 0;
}