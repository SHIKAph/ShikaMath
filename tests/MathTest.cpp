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
                canvas.PutPixel(x, y, color);
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

    // --- Triangle ---
    Vector3 vertices[3] = {
        { 0.0f, 1.0f, 0.0f}, // Top
        { 1.0f, -1.0f, 0.0f}, // Right Bottom
        { -1.0f, -1.0f, 0.0f}, // Left Bottom
    };

    // --- MVP Matrix ---
    Matrix4x4 matWorld = Matrix4x4::RotationY(ToRadian(30.0f)) * Matrix4x4::Translation({0, 0, 4.0f});
    Matrix4x4 matView = Matrix4x4::LookAtLH({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    Matrix4x4 matProj = Matrix4x4::PerspectiveFovLH(ToRadian(60.0f), (float)width/height, 0.1f, 100.0f);

    Matrix4x4 matMVP = matWorld * matView * matProj;

    // --- Vertex Processing ---
    Point2D screenPoints[3];

    for (int i = 0; i < 3; ++i) {
        Vector3 v = vertices[i];

        // MVP Transform
        __m128 raw = Matrix4x4::TransformVector(v, matMVP);
        
        alignas(16) float res[4];
        _mm_store_ps(res, raw);

        float x = res[0];
        float y = res[1];
        float z = res[2];
        float w = res[3];

        // Perspective Divide
        if (w != 0.0f) {
            x /= w;
            y /= w;
            z /= w; 
        }

        // Viewport Transform
        int screenX = (int)((x + 1.0f) * 0.5f * width);
        int screenY = (int)((1.0f - y) * 0.5f * height);

        screenPoints[i] = { screenX, screenY };

        // print coordinates for debug
        printf("V%d: (%.1f, %.1f, %.1f) -> Screen(%d, %d)\n", i, v.x, v.y, v.z, screenX, screenY);
    }

    // Draw (Rasterization)
    Vector3 p0(screenPoints[0].x, screenPoints[0].y, 0.0f);
    Vector3 p1(screenPoints[1].x, screenPoints[1].y, 0.0f);
    Vector3 p2(screenPoints[2].x, screenPoints[2].y, 0.0f);
    
    DrawFilledTriangle(canvas, p0, p1, p2, Color::Red());

    canvas.SaveToPPM("solid_triangle.ppm");

    return 0;
}