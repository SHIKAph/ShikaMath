#pragma once

#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h"
#include "../include/Matrix4x4.h"

namespace Shika{

    struct Point2D { int x, y; };

    class Rasterizer {
    public:
        // --- Draw Functions ---
        static void DrawFilledTriangle(Canvas& canvas, const Vector3& v0, const Vector3& v1, const Vector3& v2, Color color);
        static void DrawLine(Canvas& canvas, Point2D p1, Point2D p2, Color color);
        
        
        // --- Utils ---
        static Vector3 CalculateFaceNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2);
        // 3D World Coordinate -> Screen Coordinate & Depth (Return Vector3 (x: screenX, y: screenY, z: depth))
        static Vector3 TransformVertex(const Vector3& vertex, const Matrix4x4& mvpMatrix, int width, int height);
    };
}