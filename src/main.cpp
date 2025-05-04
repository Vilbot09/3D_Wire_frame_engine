#include "relay/rle.hpp"

rle::vector3d v1 = { 0.0f, 0.0f, 20.0f, 1.0f }; // Last value should always be one
rle::vector3d v2 = { 100.0f, 50.0f, 18.0f, 1.0f };
rle::vector3d v3 = { 200.0f, 20.0f, 19.0f, 1.0f };
rle::triangle3d t1 = { 0.0f, 0.0f, 20.0f, 1.0f,     10.0f, 5.0f, 18.0f, 1.0f,     20.0f, 20.0f, 19.0f, 1.0f };


rle::camera3d camera;
rle::object3d testObject;


void rle::UserStart(const appState& state) {
    testObject.mesh = LoadMeshFromObjectFile("../meshes/ship.obj");
    testObject.pos.z = 5.0f;
}

void rle::UserUpdate(const appState& state) {
    camera.GetNewWindowSize(state.window);

    SetRenderColor({255, 255, 255, 255});
    RenderObject3D(camera, testObject);

    testObject.pos.z += 1.0f * state.deltaTime;
    testObject.rot.x += 1.0f * state.deltaTime;
    // std::cout << state.frameRate << std::endl;
}

void rle::UserInput(const appState& state) {
    // std::cout << state.frameRate << std::endl;
}

int main() {
    rle::Init();
    rle::Run();
    return 0;
}