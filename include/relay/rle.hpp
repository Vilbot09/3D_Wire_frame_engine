#pragma once

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

    struct camera3d {
        float fov, fovRad, near, far;
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

    void RenderMesh3D(mesh3d);
    void RenderTriangle3D(camera3d, triangle3d);
    void RenderTriangle2D(vector2d, vector2d, vector2d);

    class engine3d { // The engine class that will the inherited
    public:
        engine3d();
    
        void Start();

    protected:
        virtual void OnUserCreate();
        virtual void OnUserUpdate();
    };
}