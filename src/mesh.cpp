#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <cmath>
#include "headers/mesh.h"



Vec3D::Vec3D() {
    x = y = z = 0.0f;
    w = 1.0f;
}

Vec3D::Vec3D(float a, float b, float c) {
    x = a; y = b; z = c;
    w = 1.0f;
}




float Vec3D::Get_Length() const {
    return std::sqrt(x*x + y*y + z*z);
}

float Vec3D::Product_Dot(const Vec3D& p) const {
    return x*p.x + y*p.y + z*p.z;
}

Vec3D Vec3D::Product_Cross(const Vec3D& p) const {
    const float inX = this->x;
    const float inY = this->y;
    const float inZ = this->z;
    
    Vec3D out;
    out.x = inY * p.z - inZ * p.y;
    out.y = inZ * p.x - inX * p.z;
    out.z = inX * p.y - inY * p.x;
    return out;
}

Vec3D& Vec3D::Normalize() {
    float len = Get_Length();
    if (len != 0.0f) {
        this->x /= len;
        this->y /= len;
        this->z /= len;
    }
    return *this;
}

Vec3D Vec3D::Intersection_Plane(Vec3D& plane_n, Vec3D& line_start, Vec3D& line_end) {
    plane_n.Normalize();
    float plane_d = -plane_n.Product_Dot(*this);
    float ad = line_start.Product_Dot(plane_n);
    float bd = line_end.Product_Dot(plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    Vec3D lineStartToEnd = line_end - line_start;
    Vec3D lineToIntersect = lineStartToEnd * t;
    return line_start + lineToIntersect;
}



Vec3D& Vec3D::operator+=(const Vec3D& p) {
    this->x += p.x;
    this->y += p.y;
    this->z += p.z;
    return *this;
}

Vec3D& Vec3D::operator-=(const Vec3D& p) {
    this->x -= p.x;
    this->y -= p.y;
    this->z -= p.z;
    return *this;
}

Vec3D& Vec3D::operator*=(const Vec3D& p) {
    this->x *= p.x;
    this->y *= p.y;
    this->z *= p.z;
    return *this;
}

Vec3D& Vec3D::operator/=(const Vec3D& p) {
    this->x /= p.x;
    this->y /= p.y;
    this->z /= p.z;
    return *this;
}





Vec3D& Vec3D::operator*=(const float& f) {
    this->x *= f;
    this->y *= f;
    this->z *= f;
    return *this;
}

Vec3D& Vec3D::operator/=(const float& f) {
    this->x /= f;
    this->y /= f;
    this->z /= f;
    return *this;
}

Vec3D& Vec3D::operator+=(const float& f) {
    this->x += f;
    this->y += f;
    this->z += f;
    return *this;
}

Vec3D& Vec3D::operator-=(const float& f) {
    this->x -= f;
    this->y -= f;
    this->z -= f;
    return *this;
}




Vec3D& Vec3D::operator*=(const Mat4x4& m) {
    const float inX = this->x;
    const float inY = this->y;
    const float inZ = this->z;

    this->x = inX * m.mat[0][0] + inY * m.mat[1][0] + inZ * m.mat[2][0] + m.mat[3][0];
    this->y = inX * m.mat[0][1] + inY * m.mat[1][1] + inZ * m.mat[2][1] + m.mat[3][1];
    this->z = inX * m.mat[0][2] + inY * m.mat[1][2] + inZ * m.mat[2][2] + m.mat[3][2];
    this->w = inX * m.mat[0][3] + inY * m.mat[1][3] + inZ * m.mat[2][3] + m.mat[3][3];
    return *this;
}




Vec3D Vec3D::operator-(const Vec3D& p) const {
    Vec3D newPoint = Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint -= p;
    return newPoint;
}

Vec3D Vec3D::operator*(const float& f) const {
    Vec3D newPoint = Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint *= f;
    return newPoint;
}

Vec3D Vec3D::operator+(const Vec3D& f) const {
    Vec3D newPoint = Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint += f;
    return newPoint;
}

Vec3D Vec3D::operator*(const Mat4x4& m) const {
    Vec3D newPoint = Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint *= m;
    return newPoint;
}




int Tri3D::ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Tri3D& out_tri1, Tri3D& out_tri2) {
    plane_n.Normalize();

    // Shortested distance from point to plane
    auto dist = [&](Vec3D& p) {
        Vec3D n = p;
        n.Normalize();
        return (plane_n.x * n.x + plane_n.y * n.y + plane_n.z * n.z - plane_n.Product_Dot(plane_p));
    };

    Vec3D* inside_points[3]; int nInsidePointCount = 0;
    Vec3D* outside_points[3]; int nOutsidePointCount = 0;

    // Distance of each point
    float d0 = dist(vecs[0]);
    float d1 = dist(vecs[1]);
    float d2 = dist(vecs[2]);

    // Fill the two arrays with points
    if (d0 >= 0) { inside_points[nInsidePointCount++] = &vecs[0]; }
    else { outside_points[nOutsidePointCount++] = &vecs[0]; }
    if (d1 >= 0) { inside_points[nInsidePointCount++] = &vecs[1]; }
    else { outside_points[nOutsidePointCount++] = &vecs[1]; }
    if (d2 >= 0) { inside_points[nInsidePointCount++] = &vecs[2]; }
    else { outside_points[nOutsidePointCount++] = &vecs[2]; }

    if (nInsidePointCount == 0) {
        // All point are outside the plane
        // The entire things is clipped

        return 0; // No valid triangles
    }

    if (nInsidePointCount == 3) {
        // All points are valid
        // No Clipping

        out_tri1 = *this;

        return 1; // One valid triangle
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
        // Clipped into a smaller triangle
        // Copy visuals over (maybe)

        out_tri1.vecs[0] = *inside_points[0]; // The inside point
        out_tri1.vecs[1] = plane_p.Intersection_Plane(plane_n, *inside_points[0], *outside_points[0]);
        out_tri1.vecs[2] = plane_p.Intersection_Plane(plane_n, *inside_points[0], *outside_points[1]);

        return 1; // One valid triangle
    }  

    if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
        // Triangle should be clipped. As two points lie inside the plane,
        // the clipped triangle becomes a "quad". Fortunately, we can
        // represent a quad with two new triangles

        // Copy appearance info to new triangles

        // The first triangle consists of the two inside points and a new
        // point determined by the location where one side of the triangle
        // intersects with the plane
        out_tri1.vecs[0] = *inside_points[0];
        out_tri1.vecs[1] = *inside_points[1];
        out_tri1.vecs[2] = plane_p.Intersection_Plane(plane_n, *inside_points[0], *outside_points[0]);

        // The second triangle is composed of one of he inside points, a
        // new point determined by the intersection of the other side of the 
        // triangle and the plane, and the newly created point above
        out_tri2.vecs[0] = *inside_points[1];
        out_tri2.vecs[1] = out_tri1.vecs[2];
        out_tri2.vecs[2] = plane_p.Intersection_Plane(plane_n, *inside_points[1], *outside_points[0]);

        return 2; // Two valid triangles
	}
}

Tri3D& Tri3D::operator*=(const Mat4x4& m) {
    this->vecs[0] *= m;
    this->vecs[1] *= m;
    this->vecs[2] *= m;
    return *this;
}

Tri3D& Tri3D::operator+=(const float& f) {
    this->vecs[0] += f;
    this->vecs[1] += f;
    this->vecs[2] += f;
    return *this;
}





bool Mesh3D::LoadFromObjectFile(std::string filename) {
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