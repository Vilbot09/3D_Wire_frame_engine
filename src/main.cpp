#include "relay/rle.hpp"

rle::vector3d v1 = { 0.0f, 0.0f, 20.0f, 1.0f }; // Last value should always be one
rle::vector3d v2 = { 100.0f, 50.0f, 18.0f, 1.0f };
rle::vector3d v3 = { 200.0f, 20.0f, 19.0f, 1.0f };

rle::triangle3d t1 = { 0.0f, 0.0f, 20.0f, 1.0f,     10.0f, 5.0f, 18.0f, 1.0f,     20.0f, 20.0f, 19.0f, 1.0f };
rle::camera3d camera;

rle::mesh3d testMesh = rle::LoadMeshFromObjectFile("../meshes/ship.obj");

void rle::UserStart(rle::appState state) {

}

void rle::UserUpdate(rle::appState state) {

    SetRenderColor({255, 255, 255, 255});
    RenderMesh3D(camera, testMesh);
}

void rle::UserInput(rle::appState state) {

}

int main() {
    rle::Init();
    rle::Run();
    return 0;
}