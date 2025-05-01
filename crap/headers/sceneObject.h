#pragma once

#include "headers/mesh.h"

class SceneObject {
public:
    Vec3D vPos;
    Vec3D vRot;

    Mesh3D mesh;

    SceneObject() {
        vPos.x = 0.0f; vPos.y = 0.0f; vPos.z = 0.0f;
    }

    void LoadMeshFromObjectFile(std::string filename) {
        mesh.LoadFromObjectFile(filename);
    }
};
