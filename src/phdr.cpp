#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <cmath>
#include "headers/phdr.h"

//******
// MAT 4x4
//******

phdr::Mat4x4 phdr::Mat4x4::operator*(const phdr::Mat4x4& m) const {
    phdr::Mat4x4 newMat;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            newMat.mat[row][col] = 
                this->mat[row][0] * m.mat[0][col] +
                this->mat[row][1] * m.mat[1][col] +
                this->mat[row][2] * m.mat[2][col] +
                this->mat[row][3] * m.mat[3][col];
        }
    }

    return newMat;
}

// ******
// VEC 3D
// ******

phdr::Vec3D::Vec3D() {
    x = y = z = 0.0f;
    w = 1.0f;
}

phdr::Vec3D::Vec3D(float a, float b, float c) {
    x = a; y = b; z = c;
    w = 1.0f;
}

float phdr::Vec3D::Get_Length() const {
    return std::sqrt(x*x + y*y + z*z);
}

float phdr::Vec3D::Product_Dot(const phdr::Vec3D& p) const {
    return x*p.x + y*p.y + z*p.z;
}

phdr::Vec3D phdr::Vec3D::Product_Cross(const phdr::Vec3D& p) const {
    const float inX = this->x;
    const float inY = this->y;
    const float inZ = this->z;
    
    phdr::Vec3D out;
    out.x = inY * p.z - inZ * p.y;
    out.y = inZ * p.x - inX * p.z;
    out.z = inX * p.y - inY * p.x;
    return out;
}

phdr::Vec3D& phdr::Vec3D::Normalize() {
    float len = Get_Length();
    if (len != 0.0f) {
        this->x /= len;
        this->y /= len;
        this->z /= len;
    }
    return *this;
}

phdr::Vec3D phdr::Vec3D::Intersection_Plane(phdr::Vec3D& plane_n, phdr::Vec3D& line_start, phdr::Vec3D& line_end) {
    plane_n.Normalize();
    float plane_d = -plane_n.Product_Dot(*this);
    float ad = line_start.Product_Dot(plane_n);
    float bd = line_end.Product_Dot(plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    Vec3D lineStartToEnd = line_end - line_start;
    Vec3D lineToIntersect = lineStartToEnd * t;
    return line_start + lineToIntersect;
}

phdr::Vec3D& phdr::Vec3D::operator+=(const phdr::Vec3D& p) {
    this->x += p.x;
    this->y += p.y;
    this->z += p.z;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator-=(const phdr::Vec3D& p) {
    this->x -= p.x;
    this->y -= p.y;
    this->z -= p.z;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator*=(const phdr::Vec3D& p) {
    this->x *= p.x;
    this->y *= p.y;
    this->z *= p.z;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator/=(const phdr::Vec3D& p) {
    this->x /= p.x;
    this->y /= p.y;
    this->z /= p.z;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator*=(const float& f) {
    this->x *= f;
    this->y *= f;
    this->z *= f;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator/=(const float& f) {

    this->x /= f;
    this->y /= f;
    this->z /= f;

    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator+=(const float& f) {
    this->x += f;
    this->y += f;
    this->z += f;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator-=(const float& f) {
    this->x -= f;
    this->y -= f;
    this->z -= f;
    return *this;
}

phdr::Vec3D& phdr::Vec3D::operator*=(const phdr::Mat4x4& m) {
    const float inX = this->x;
    const float inY = this->y;
    const float inZ = this->z;

    this->x = inX * m.mat[0][0] + inY * m.mat[1][0] + inZ * m.mat[2][0] + m.mat[3][0];
    this->y = inX * m.mat[0][1] + inY * m.mat[1][1] + inZ * m.mat[2][1] + m.mat[3][1];
    this->z = inX * m.mat[0][2] + inY * m.mat[1][2] + inZ * m.mat[2][2] + m.mat[3][2];
    this->w = inX * m.mat[0][3] + inY * m.mat[1][3] + inZ * m.mat[2][3] + m.mat[3][3];
    return *this;
}

phdr::Vec3D phdr::Vec3D::operator-(const phdr::Vec3D& p) const {
    Vec3D newPoint = Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint -= p;
    return newPoint;
}

phdr::Vec3D phdr::Vec3D::operator*(const float& f) const {
    phdr::Vec3D newPoint = phdr::Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint *= f;
    return newPoint;
}

phdr::Vec3D phdr::Vec3D::operator+(const phdr::Vec3D& f) const {
    phdr::Vec3D newPoint = phdr::Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint += f;
    return newPoint;
}

phdr::Vec3D phdr::Vec3D::operator*(const phdr::Mat4x4& m) const {
    phdr::Vec3D newPoint = phdr::Vec3D(this->x, this->y, this->z);
    newPoint.w = this->w;
    newPoint *= m;
    return newPoint;
}

int phdr::Tri3D::ClipAgainstPlane(phdr::Vec3D plane_p, phdr::Vec3D plane_n, phdr::Tri3D& out_tri1, phdr::Tri3D& out_tri2) {
    plane_n.Normalize();

    // Shortested distance from point to plane
    auto dist = [&](phdr::Vec3D& p) {
        phdr::Vec3D n = p;
        n.Normalize();
        return (plane_n.x * n.x + plane_n.y * n.y + plane_n.z * n.z - plane_n.Product_Dot(plane_p));
    };

    phdr::Vec3D* inside_points[3]; int nInsidePointCount = 0;
    phdr::Vec3D* outside_points[3]; int nOutsidePointCount = 0;

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

    return 0;
}




// ******
// TRI 3D
// ******

phdr::Tri3D& phdr::Tri3D::operator*=(const phdr::Mat4x4& m) {
    this->vecs[0] *= m;
    this->vecs[1] *= m;
    this->vecs[2] *= m;
    return *this;
}

phdr::Tri3D& phdr::Tri3D::operator+=(const float& f) {
    this->vecs[0] += f;
    this->vecs[1] += f;
    this->vecs[2] += f;
    return *this;
}

bool phdr::Mesh3D::LoadFromObjectFile(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::vector<phdr::Vec3D> verts;

    while (!file.eof()) {  // eof for "end of file"
        char line[128];
        file.getline(line, 128);

        std::strstream stream;
        stream << line;

        char junk;

        if (line[0] == 'v') { // We are reading a vertex
            phdr::Vec3D vertex;
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



// ******
// ENGINE
// ******

void phdr::Engine::Start() {
    InitWindow(500, 500, "Engine");
    SetTargetFPS(60);

    OnUserCreate();

    camera.fAspectRatio = (float)camera.GetHeight()/(float)camera.GetWidth();

    while (!WindowShouldClose()) {
        

        BeginDrawing();
            ClearBackground(BLACK);
            OnUserUpdate();
        EndDrawing();
    }

    CloseWindow();
}

void phdr::Engine::OnUserCreate() {}
void phdr::Engine::OnUserUpdate() {}

void phdr::Engine::RenderTriangle2D(float x1, float y1, float x2, float y2, float x3, float y3) {
    DrawLine(x1, y1, x2, y2, GRAY);
    DrawLine(x2, y2, x3, y3, GRAY);
    DrawLine(x3, y3, x1, y1, GRAY);
}

void phdr::Engine::RenderTriangle3D(phdr::Tri3D tri, phdr::SceneObject obj, phdr::Mat4x4 combined) {
    phdr::Tri3D newTri;
    newTri = tri;

    newTri *= combined;

    int nClippedTriangles = 0;
    phdr::Tri3D clipped[2];
    nClippedTriangles = newTri.ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, clipped[0], clipped[1]);

    // Projecting the clipped triangles
    for (int n = 0; n < nClippedTriangles; n++) {
        clipped[n] *= ProjectionMatrix(camera); 

        // Divide by W
        clipped[n].vecs[0] /= clipped[n].vecs[0].w;
        clipped[n].vecs[1] /= clipped[n].vecs[1].w;
        clipped[n].vecs[2] /= clipped[n].vecs[2].w;
    
        // Scale into view
        clipped[n] += 1.0f;
        clipped[n].vecs[0].x *= camera.GetWidth() * 0.5f; clipped[n].vecs[0].y *= camera.GetHeight() * 0.5f;
        clipped[n].vecs[1].x *= camera.GetWidth() * 0.5f; clipped[n].vecs[1].y *= camera.GetHeight() * 0.5f;
        clipped[n].vecs[2].x *= camera.GetWidth() * 0.5f; clipped[n].vecs[2].y *= camera.GetHeight() * 0.5f;
    
        RenderTriangle2D(
            clipped[n].vecs[0].x, clipped[n].vecs[0].y,
            clipped[n].vecs[1].x, clipped[n].vecs[1].y,
            clipped[n].vecs[2].x, clipped[n].vecs[2].y
        );
    }
}

void phdr::Engine::RenderSceneObject(phdr::SceneObject obj) {
    // Probably not even here
    vUp = phdr::Vec3D(0, 1, 0); 
    vTarget = phdr::Vec3D(0, 0, 1); // Reset up and vTarget each frame
    phdr::Mat4x4 camRotMatY = RotationMatrixY(camera.fYaw);
    phdr::Mat4x4 camRotMatX = RotationMatrixX(camera.fPitch);

    phdr::Vec3D vLookDir = vTarget * camRotMatX;
    vLookDir *= camRotMatY;
    camera.vLookDir = vLookDir;
    vTarget = camera.vPos + camera.vLookDir;

    // Combined matrix
    phdr::Mat4x4 combined;
    //combined = QuickInvertMatrix(PointAtMatrix(camera.vPos, vTarget, vUp));
    combined = TranslationMatrix(obj.vPos.x, obj.vPos.y, obj.vPos.z);
    combined = combined * RotationMatrixZ(obj.vRot.z);
    combined = combined * RotationMatrixY(obj.vRot.y);
    combined = combined * RotationMatrixX(obj.vRot.x);

    for (const auto& tri : obj.mesh.tris) {
        RenderTriangle3D(tri, obj, combined);
    }
}

phdr::Mat4x4 phdr::Engine::RotationMatrixX(float rad) {
    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = 1;
    matrix.mat[1][1] = std::cos(rad);
    matrix.mat[2][2] = std::cos(rad);
    matrix.mat[2][1] = std::sin(rad);
    matrix.mat[1][2] = -std::sin(rad);
    return matrix;
}

phdr::Mat4x4 phdr::Engine::RotationMatrixY(float rad) {
    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = std::cos(rad);
    matrix.mat[1][1] = 1;
    matrix.mat[2][2] = std::cos(rad);
    matrix.mat[0][2] = std::sin(rad);
    matrix.mat[2][0] = -std::sin(rad);
    return matrix;
}

phdr::Mat4x4 phdr::Engine::RotationMatrixZ(float rad) {
    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = std::cos(rad);
    matrix.mat[1][1] = std::cos(rad);
    matrix.mat[2][2] = 1;
    matrix.mat[1][0] = std::sin(rad);
    matrix.mat[0][1] = -std::sin(rad);
    return matrix;
}

phdr::Mat4x4 phdr::Engine::TranslationMatrix(float tX, float tY, float tZ) {
    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = 1.0f;
    matrix.mat[1][1] = 1.0f;
    matrix.mat[2][2] = 1.0f;
    matrix.mat[3][3] = 1.0f;
    matrix.mat[3][0] = tX;
    matrix.mat[3][1] = tY;
    matrix.mat[3][2] = tZ;
    return matrix;
}

phdr::Mat4x4 phdr::Engine::ProjectionMatrix(phdr::CameraObject camera) {
    float fRatio = camera.fFar / (camera.fFar - camera.fNear);

    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = camera.fAspectRatio * camera.fFovRad;
    matrix.mat[1][1] = camera.fFovRad;
    matrix.mat[2][2] = fRatio;
    matrix.mat[3][2] = -camera.fNear * fRatio;
    matrix.mat[2][3] = 1.0f;
    matrix.mat[3][3] = 0.0f;
    return matrix;
}

phdr::Mat4x4 phdr::Engine::PointAtMatrix(phdr::Vec3D vPos, phdr::Vec3D target, phdr::Vec3D up) {
    phdr::Vec3D newForward = target - vPos;
    newForward.Normalize();

    float dotProduct = up.Product_Dot(newForward);
    phdr::Vec3D a = newForward * dotProduct;
    phdr::Vec3D newUp = up - a;
    newUp.Normalize();
    //std::cout << newUp.y << std::endl;
    //std::cout.flush();
    // std cout med flush förstör framRaten ...

    phdr::Vec3D newRight = newUp.Product_Cross(newForward);

    phdr::Mat4x4 newMat;
    newMat.mat[0][0] = newRight.x;       newMat.mat[0][1] = newRight.y;       newMat.mat[0][2] = newRight.z;      newMat.mat[0][3] = 0.0f;
    newMat.mat[1][0] = newUp.x;          newMat.mat[1][1] = newUp.y;          newMat.mat[1][2] = newUp.z;         newMat.mat[1][3] = 0.0f;
    newMat.mat[2][0] = newForward.x;     newMat.mat[2][1] = newForward.y;     newMat.mat[2][2] = newForward.z;    newMat.mat[2][3] = 0.0f;
    newMat.mat[3][0] = vPos.x;           newMat.mat[3][1] = vPos.y;           newMat.mat[3][2] = vPos.z;          newMat.mat[3][3] = 1.0f;
    return newMat;
}

phdr::Mat4x4 phdr::Engine::QuickInvertMatrix(phdr::Mat4x4 m) {
    phdr::Mat4x4 matrix;
    matrix.mat[0][0] = m.mat[0][0]; matrix.mat[0][1] = m.mat[1][0]; matrix.mat[0][2] = m.mat[2][0]; matrix.mat[0][3] = 0.0f;
    matrix.mat[1][0] = m.mat[0][1]; matrix.mat[1][1] = m.mat[1][1]; matrix.mat[1][2] = m.mat[2][1]; matrix.mat[1][3] = 0.0f;
    matrix.mat[2][0] = m.mat[0][2]; matrix.mat[2][1] = m.mat[1][2]; matrix.mat[2][2] = m.mat[2][2]; matrix.mat[2][3] = 0.0f;
    matrix.mat[3][0] = -(m.mat[3][0] * matrix.mat[0][0] + m.mat[3][1] * matrix.mat[1][0] + m.mat[3][2] * matrix.mat[2][0]);
    matrix.mat[3][1] = -(m.mat[3][0] * matrix.mat[0][1] + m.mat[3][1] * matrix.mat[1][1] + m.mat[3][2] * matrix.mat[2][1]);
    matrix.mat[3][2] = -(m.mat[3][0] * matrix.mat[0][2] + m.mat[3][1] * matrix.mat[1][2] + m.mat[3][2] * matrix.mat[2][2]);
    matrix.mat[3][3] = 1.0f;
    return matrix;
}