#pragma once

#include "headers/mesh.h"

class SceneObject {
public:
    Vec3D pos;
    Vec3D rot;

    Mesh3D mesh;

    SceneObject() {
        pos.x = 0.0f; pos.y = 0.0f; pos.z = 0.0f;
    }

    void loadMeshFromObjectFile(std::string filename) {
        mesh.LoadFromObjectFile(filename);
    }
};
