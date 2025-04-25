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
    Vec3D(float a, float b, float c);

    float Get_Length() const;
    float Product_Dot(const Vec3D& p) const;
    Vec3D Product_Cross(const Vec3D& p) const;
    Vec3D& Normalize();
    
    Vec3D& operator+=(const Vec3D& p);
    Vec3D& operator-=(const Vec3D& p);
    Vec3D& operator*=(const Vec3D& p);
    Vec3D& operator/=(const Vec3D& p);

    Vec3D& operator+=(const float& f);
    Vec3D& operator-=(const float& f);
    Vec3D& operator*=(const float& f);
    Vec3D& operator/=(const float& f);

    Vec3D& operator*=(const Mat4x4& m);

    Vec3D operator-(const Vec3D& p) const;
    Vec3D operator+(const Vec3D& f) const;
    Vec3D operator*(const float& f) const;
    Vec3D operator*(const Mat4x4& m) const;
};

struct Tri3D {
    Vec3D vecs[3];

    Tri3D& operator*=(const Mat4x4& m);
    Tri3D& operator+=(const float& f);
};

struct Mesh3D {
    std::vector<Tri3D> tris;

    bool LoadFromObjectFile(std::string filename);
};
