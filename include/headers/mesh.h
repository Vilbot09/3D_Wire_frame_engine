#pragma once

#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <cmath>

struct Mat4x4 {
    float mat[4][4] = {0};
};

struct Vec3D {
    //float x, y, z;

    union {
        struct {float x, y, z;};
    };

    Vec3D() {
        x = y = z = 0;
    }

    Vec3D(float a, float b, float c) {
        x = a; y = b; z = c;
    }

    float length() {
        return std::sqrt(x*x + y*y + z*z);
    }

    float dot(const Vec3D& p) {
        return x*p.x + y*p.y + z*p.z;
    }

    Vec3D cross(const Vec3D& p) {
        return Vec3D(
            y * p.z - z * p.y,
            z * p.x - x * p.z,
            x * p.y - y * p.x
        );
    }

    Vec3D& operator+=(const Vec3D& p) {
        this->x += p.x;
        this->y += p.y;
        this->z += p.z;
        return *this;
    }

    Vec3D& operator-=(const Vec3D& p) {
        this->x -= p.x;
        this->y -= p.y;
        this->z -= p.z;
        return *this;
    }

    Vec3D& operator*=(const Vec3D& p) {
        this->x *= p.x;
        this->y *= p.y;
        this->z *= p.z;
        return *this;
    }

    Vec3D& operator/=(const Vec3D& p) {
        this->x /= p.x;
        this->y /= p.y;
        this->z /= p.z;
        return *this;
    }

    Vec3D& operator*=(const Mat4x4& m) {
        const float inX = this->x;
        const float inY = this->y;
        const float inZ = this->z;

        this->x = inX * m.mat[0][0] + inY * m.mat[1][0] + inZ * m.mat[2][0] + m.mat[3][0];
        this->y = inX * m.mat[0][1] + inY * m.mat[1][1] + inZ * m.mat[2][1] + m.mat[3][1];
        this->z = inX * m.mat[0][2] + inY * m.mat[1][2] + inZ * m.mat[2][2] + m.mat[3][2];
        return *this;
    }
};

struct Tri3D {
    Vec3D vecs[3];

    Tri3D& operator*=(const Mat4x4& m) {
        this->vecs[0] *= m;
        this->vecs[1] *= m;
        this->vecs[2] *= m;
        return *this;
    }
};

struct Mesh3D {
    std::vector<Tri3D> tris;

    bool LoadFromObjectFile(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::vector<Vec3D> verts;

        while (!file.eof()) {  // eof for "end of file"
            char line[128];
            file.getline(line, 128);

            std::strstream stream;
            stream << line;

            char junk;

            if (line[0] == 'v') { // We are reading a vertex
                Vec3D vertex;
                stream >> junk >> vertex.x >> vertex.y >> vertex.z;
                verts.push_back(vertex);
            }

            if (line[0] == 'f') {
                int face[3];
                stream >> junk >> face[0] >> face[1] >> face[2];

                tris.push_back({
                    verts[face[0]-1],
                    verts[face[1]-1],
                    verts[face[2]-1]
                });
            }

        }

        return true;
    }
};
