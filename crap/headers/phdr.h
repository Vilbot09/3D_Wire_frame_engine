#pragma once

#include <string>
#include <vector>
#include <cmath>
#include "raylib/raylib.h"

namespace phdr {
    struct Mat4x4 {
        float mat[4][4] = {0};

        Mat4x4 operator*(const Mat4x4&) const;
    };

    struct Vec3D {
        union {
            struct {float x, y, z, w;};
        };
    
        Vec3D();
        Vec3D(float, float, float);
    
        float Get_Length() const;
        float Product_Dot(const Vec3D&) const;
        Vec3D Product_Cross(const Vec3D&) const;
        Vec3D& Normalize();
        Vec3D Intersection_Plane(Vec3D&, Vec3D&, Vec3D&);
        
        Vec3D& operator+=(const Vec3D&);
        Vec3D& operator-=(const Vec3D&);
        Vec3D& operator*=(const Vec3D&);
        Vec3D& operator/=(const Vec3D&);
    
        Vec3D& operator+=(const float&);
        Vec3D& operator-=(const float&);
        Vec3D& operator*=(const float&);
        Vec3D& operator/=(const float&);
    
        Vec3D& operator*=(const Mat4x4&);
    
        Vec3D operator-(const Vec3D&) const;
        Vec3D operator+(const Vec3D&) const;
        Vec3D operator*(const float&) const;
        Vec3D operator*(const Mat4x4&) const;
    };

    struct Tri3D {
        Vec3D vecs[3];
    
        int ClipAgainstPlane(Vec3D, Vec3D, Tri3D&, Tri3D&);
    
        Tri3D& operator*=(const Mat4x4&);
        Tri3D& operator+=(const float&);
    };
    
    struct Mesh3D {
        std::vector<Tri3D> tris;
    
        bool LoadFromObjectFile(std::string);
    };

    class CameraObject {
    public:
        Vec3D vPos;
        Vec3D vLookDir;
    
        float fYaw;
        float fPitch;
    
        float fSpeed;
        float fPitchSpeed;
        float fYawSpeed;
    
        float fAspectRatio;
        float fFov;
        float fFovRad;
        float fNear;
        float fFar;
    
        CameraObject() {
            fSpeed = 0.3f;
            fYawSpeed = 0.04f;
            fPitchSpeed = 0.03f;
    
            fYaw = 0.0f;
            fPitch = 0.0f;
            vLookDir = Vec3D(0, 0, 1);
    
            fAspectRatio = (float)GetHeight()/(float)GetWidth();
            fFov = 70.0f;
            fFovRad = 1.0f / std::tan(fFov * 0.5f / 180.0f * 3.141592f);
            fNear = 0.1f;
            fFar = 1000.0f;
        }
    
        int GetWidth() {return GetScreenWidth();}
        int GetHeight() {return GetScreenHeight();}
    };

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

    class Engine {
    public:
        Engine() = default;

        void Start();

    protected:
        Vec3D vUp;
        Vec3D vTarget;
        CameraObject camera;

        virtual void OnUserCreate();
        virtual void OnUserUpdate();

        void RenderTriangle2D(float, float, float, float, float, float);
        void RenderTriangle3D(Tri3D, SceneObject, Mat4x4);
        void RenderSceneObject(SceneObject);

        Mat4x4 RotationMatrixX(float);
        Mat4x4 RotationMatrixY(float);
        Mat4x4 RotationMatrixZ(float);
        Mat4x4 TranslationMatrix(float, float, float);
        Mat4x4 ProjectionMatrix(CameraObject);
        Mat4x4 PointAtMatrix(Vec3D, Vec3D, Vec3D);
        Mat4x4 QuickInvertMatrix(Mat4x4);
    };
}