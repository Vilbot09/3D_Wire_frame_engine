#pragma once

#include <string>
#include <vector>

struct Mat4x4 {
    float mat[4][4] = {0};
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
