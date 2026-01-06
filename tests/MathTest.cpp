#include <iostream>
#include <cstdio>
#include <cmath> 
#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 

using namespace Shika;

struct Point2D { int x, y; };

inline float EdgeFunction(const Vector3& v0, const Vector3& v1, const Vector3& p) {
    return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

Vector3 CalculateFaceNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2) {
           Vector3 edge1 = v1 - v0;
           Vector3 edge2 = v2 - v0;
    
           // Cross Product -> Normalize
           return edge1.Cross(edge2).Normalized();
};

void DrawFilledTriangle(Canvas& canvas, const Vector3& v0, const Vector3& v1, const Vector3& v2, Color color) {
    // Calculate Bounding Box
    int minX = (int)std::floor(std::min({v0.x, v1.x, v2.x}));
    int minY = (int)std::floor(std::min({v0.y, v1.y, v2.y}));
    int maxX = (int)std::ceil(std::max({v0.x, v1.x, v2.x}));
    int maxY = (int)std::ceil(std::max({v0.y, v1.y, v2.y}));

    // Clipping
    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, canvas.GetWidth() - 1);
    maxY = std::min(maxY, canvas.GetHeight() - 1);

    // The Area of the Triangle
    float area = EdgeFunction(v0, v1, v2);

    if (area >= 0) return;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // Miidle point of Pixel (+0.5f)
            Vector3 p((float)x + 0.5f, (float)y + 0.5f, 0.0f);

            // Edge Function Test about 3 sides
            float w0 = EdgeFunction(v1, v2, p); // v1 -> v2 
            float w1 = EdgeFunction(v2, v0, p); // v2 -> v0 
            float w2 = EdgeFunction(v0, v1, p); // v0 -> v1 

            // Put Pixef if pass Edge Test
            if (w0 <= 0 && w1 <= 0 && w2 <= 0) {
                // alpha + beta + gamma = 1
                float alpha = w0 / area;
                float beta  = w1 / area;
                float gamma = w2 / area;

                // Depth Interpolation
                float z = alpha * v0.z + beta * v1.z + gamma * v2.z;

                if (z < canvas.GetDepth(x, y)) {
                    canvas.SetDepth(x, y, z);      // Depth Update
                    canvas.PutPixel(x, y, color);  // Color Update
                }
            }
        }
    }
}

void DrawLine(Canvas& canvas, Point2D p1, Point2D p2, Color color) {
    int x0 = p1.x; int y0 = p1.y;
    int x1 = p2.x; int y1 = p2.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        canvas.PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

int main() {

    const int width = 400;
    const int height = 300;
    Canvas canvas(width, height);

    // --- Cube ---
    Vector3 vertices[] = {
        {-1,  1, -1}, { 1,  1, -1}, {-1, -1, -1}, { 1, -1, -1}, // Front (0,1,2,3)
        {-1,  1,  1}, { 1,  1,  1}, {-1, -1,  1}, { 1, -1,  1}  // Back (4,5,6,7)
    };

    // Cube Index Data (12 Triangles)
    // {v0, v2, v1} (CW)
    int indices[][3] = {
    {0, 2, 1}, {2, 3, 1}, // Front
    {1, 3, 5}, {3, 7, 5}, // Right
    {5, 7, 4}, {7, 6, 4}, // Back
    {4, 6, 0}, {6, 2, 0}, // Left
    {4, 0, 5}, {0, 1, 5}, // Top
    {2, 6, 3}, {6, 7, 3}  // Bottom
};

    // --- MVP Matrix ---
    Matrix4x4 matWorld = Matrix4x4::RotationX(ToRadian(30)) * Matrix4x4::RotationY(ToRadian(45)) * Matrix4x4::Translation({0, 0, 15});
    Matrix4x4 matView = Matrix4x4::LookAtLH({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    Matrix4x4 matProj = Matrix4x4::PerspectiveFovLH(ToRadian(50), (float)width/height, 0.1f, 100.0f);

    Matrix4x4 matMVP = matWorld * matView * matProj;

    // Directional Light
    Vector3 lightDir = Vector3(-1.0f, 1.0f, -1.0f).Normalized();

    // Render Loop
    for (int i = 0; i < 12; i++) {

        // 1. Triangles Vertex
        Vector3 p0 = vertices[indices[i][0]];
        Vector3 p1 = vertices[indices[i][1]];
        Vector3 p2 = vertices[indices[i][2]];

        // 2. Calculate FaceNormal
        Vector3 normal = CalculateFaceNormal(p0, p1, p2);

        // 3. Normal -> World Space
        __m128 normalRotated = Matrix4x4::TransformDirection(normal, matWorld);
        Vector3 worldNormal = Vector3(normalRotated).Normalized();

        // Light Calculates with Lambert's Raw
        float intensity = std::max(0.0f, worldNormal.Dot(lightDir));
        intensity = std::clamp(intensity + 0.1f, 0.0f, 1.0f);
        Color finalColor = {intensity, intensity, intensity};

        auto TransformAndDraw = [&](Vector3 v0, Vector3 v1, Vector3 v2, Color c) {
            Vector3 inputVerts[3] = {v0, v1, v2};
            Point2D screenPts[3];
            float depth[3];

            for(int k=0; k<3; ++k) {
                // MVP Transform
                __m128 raw = Matrix4x4::TransformVector(inputVerts[k], matMVP);
                alignas(16) float res[4]; 
                _mm_store_ps(res, raw);
                
                float x = res[0], y = res[1], z = res[2], w = res[3];
                
                // Perspective Divide
                if (w != 0.0f) { x /= w; y /= w; z /= w; } 

                // Viewport Transform
                screenPts[k].x = (int)((x + 1.0f) * 0.5f * width);
                screenPts[k].y = (int)((1.0f - y) * 0.5f * height);
                
                // Save Depth
                depth[k] = z; 
            }

            Vector3 sp0((float)screenPts[0].x, (float)screenPts[0].y, depth[0]);
            Vector3 sp1((float)screenPts[1].x, (float)screenPts[1].y, depth[1]);
            Vector3 sp2((float)screenPts[2].x, (float)screenPts[2].y, depth[2]);

            DrawFilledTriangle(canvas, sp0, sp1, sp2, c);
        };

        TransformAndDraw(p0, p1, p2, finalColor);
    }


    canvas.SaveToPPM("shaded.cube.ppm");

    return 0;
}