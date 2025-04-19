#pragma once

#include "raylib/raylib.h"
#include <vector>
#include <string>
#include <fstream>
#include <strstream>

struct Vec3D {
    float x, y, z;
};

struct Tri3D {
    Vec3D vecs[3];
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

struct Mat4x4 {
    float mat[4][4] = {0};
};

class Engine {
public:
    Mesh3D testCube;
    Mat4x4 simpleProjection;

    // Rotation Matricies
    Mat4x4 xRotMat;
    Mat4x4 yRotMat;
    Mat4x4 zRotMat;


    int width = 1000;
    int height = 700;

    float aspectRatio = height/width;
    float FOV = 70;
    float FOVRad = FOV/180 * 3.141592;
    float nearClipPlane = 0.1f;
    float farClipPlane = 100.0f;

    Engine();
    
    void multiplyMatrix4x4(Mat4x4 matrix, Vec3D in, Vec3D &out);
    void calculateNormalVector(Tri3D tri, Vec3D &out);
    void drawTriangle3D(Mat4x4 matrix, Tri3D tri);
    void drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
    void drawMesh(Mat4x4, Mesh3D);
    void drawMeshes();
};