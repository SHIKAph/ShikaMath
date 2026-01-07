#include "Rasterizer.h"
#include <algorithm> 
#include <cmath>    

namespace Shika {

    static inline float EdgeFunction(const Vector3& v0, const Vector3& v1, const Vector3& p) {
        return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
    }

    void Rasterizer::DrawFilledTriangle(Canvas& canvas, const Vector3& v0, const Vector3& v1, const Vector3& v2, Color color) {
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

    void Rasterizer::DrawLine(Canvas& canvas, Point2D p1, Point2D p2, Color color) {
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

    Vector3 Rasterizer::CalculateFaceNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2) {
        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;

        return edge1.Cross(edge2).Normalized();
    }

    Vector3 Rasterizer::TransformVertex(const Vector3& vertex, const Matrix4x4& mvpMatrix, int width, int height) {
             // 1. MVP Transform 
            __m128 raw = Matrix4x4::TransformVector(vertex, mvpMatrix);
            alignas(16) float res[4];
            _mm_store_ps(res, raw);
            
            float x = res[0], y = res[1], z = res[2], w = res[3];

            // 2. Perspective Divide
            if (w != 0.0f) { x /= w; y /= w; z /= w; }

            // 3. Viewport Transform
            float screenX = (x + 1.0f) * 0.5f * width;
            float screenY = (1.0f - y) * 0.5f * height;

            return Vector3(screenX, screenY, z);
        }
    
}

