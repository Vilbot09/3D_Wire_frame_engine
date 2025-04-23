#pragma once

#include <vector>
#include <fstream>
#include <strstream>
#include <string>

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
