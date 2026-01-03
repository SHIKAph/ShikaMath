#include <iostream>
#include <cstdio>
#include <cmath> 
#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 

using namespace Shika;

struct Point2D { int x, y; };

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

/*
    printf("=== Memory Alignment Test ===\n");
    printf("Size of Vector3: %zu bytes (Expected: 16)\n", sizeof(Vector3));
    

    static_assert(sizeof(Vector3) == 16, "Vector3 size must be 16 bytes!");
    static_assert(alignof(Vector3) == 16, "Vector3 alignment must be 16 bytes!");


    Vector3 vec;
    size_t address = reinterpret_cast<size_t>(&vec);
    printf("Address of vec: 0x%zX\n", address);
    
    if (address % 16 == 0) {
        printf("[PASS] Memory is 16-byte aligned.\n");
    } else {
        printf("[FAIL] Memory is NOT aligned! Check ALIGN16 macro.\n");
        return -1;
    }


    printf("\n=== Union & Constructor Test ===\n");
    Vector3 v1(1.0f, 2.0f, 3.0f);
    
    printf("Input: (1.0, 2.0, 3.0)\n");
    printf("Stored: x=%.1f, y=%.1f, z=%.1f\n", v1.x, v1.y, v1.z);
    
    if (v1.x == 1.0f && v1.y == 2.0f && v1.z == 3.0f) {
        printf("[PASS] Constructor & Union work correctly.\n");
    } else {
        printf("[FAIL] Values are corrupted. Check _mm_set_ps order.\n");
    }

    printf("\n=== Add, Sub Operator Test ===\n");
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a + b;

    printf("Vector3 c stored x=%.1f, y=%.1f, z=%.1f\n", c.x, c.y, c.z);


    printf("\n=== Multiplication Operator Test ===\n");
    Vector3 Vcw(2.0f, 0.0f, 1.0f);
    Vector3 scalar = a * 2.0f;
    Vector3 component = a * Vcw; 

    printf("Vector3 scalar stored x=%.1f, y=%.1f, z=%.1f\n", scalar.x, scalar.y, scalar.z);
    printf("Vector3 component stored x=%.1f, y=%.1f, z=%.1f\n", component.x, component.y, component.z);


    printf("\n=== Dot Product and Normalize Test ===\n");
    Vector3 d1(1.0f, 0.0f, 0.0f);
    Vector3 d2(0.0f, 1.0f, 0.0f);
    float dot = d1.Dot(d2);
    printf("Dot Product of (1.0f, 0.0f, 0.0f) and (0.0f, 1.0f, 0.0f): %.2f\n", dot);
    dot = d1.Dot(d1);
    printf("Dot Product of (1.0f, 0.0f, 0.0f) and (1.0f, 0.0f, 0.0f): %.2f\n", dot);
    Vector3 vLen(3.0f, 4.0f, 0.0f);
    float length = vLen.Length();
    printf("Length of (3.0f, 4.0f, 0.0f): %.2f \n", length);
    Vector3 norm = vLen * (1.0f / length);
    printf("Normalized Vector(3.0f, 4.0f, 0.0f): x=%.2f, y=%.2f, z=%.2f\n", norm.x, norm.y, norm.z);

    printf("\n=== Cross Product Test ===\n");
    Vector3 Xaxis(1.0f, 0.0f, 0.0f);
    Vector3 Yaxis(0.0f, 1.0f, 0.0f);
    Vector3 Zaxis = Xaxis.Cross(Yaxis);
    Vector3 Zaxis_n = Yaxis.Cross(Xaxis);
    printf("Cross Product of Xaxis and Yaxis: x=%.2f, y=%.2f, z=%.2f\n", Zaxis.x, Zaxis.y, Zaxis.z);
    printf("Cross Product of Yaxis and Xaxis: x=%.2f, y=%.2f, z=%.2f\n", Zaxis_n.x, Zaxis_n.y, Zaxis_n.z);


    printf("\n=== Matrix4x4 first Test ===\n");
    Matrix4x4 mat1;
    printf("Size of Matrix4x4 is %.2ld\n", sizeof(Matrix4x4));
    mat1 = Matrix4x4::Identity();
    printf("[ Identity Matrix ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", mat1.m[i][j]);
        printf("\n");
    }


   printf("\n=== Matrix Multiplication Test ===\n");
   Matrix4x4 mat1;
   mat1 = Matrix4x4::Identity();
   printf("[ Identity Matrix * Identity Matrix ]\n");
   mat1 = mat1*mat1;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", mat1.m[i][j]);
        printf("\n");
    }

   Matrix4x4 mata, matb;
   for(int i=0; i<4; i++){
    mata.row[i] = _mm_set_ps(1, 1, 1, 1);
   }
   for(int i=0; i<4; i++){
    matb.row[i] = _mm_set_ps(2, 2, 2, 2);
   }
   printf("[ 1.0 Matrix * 2.0 Matrix ]\n");
   Matrix4x4 result = mata*matb;
   for(int i=0; i<4; i++){
       for(int j=0; j<4; j++)
            printf("%.2f ", result.m[i][j]);
        printf("\n");
   }

   printf("\n=== Matrix Translation Test ===\n");
   Matrix4x4 matT;
   matT = Matrix4x4::Translation(Vector3(10, 0, 0));
   printf("[ Translation Matrices(10, 0, 0) ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matT.m[i][j]);
        printf("\n");
    }
    

    printf("\n=== View Matrix Test ===\n");
    Matrix4x4 matV;
    matV = Matrix4x4::LookAtLH(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
    Matrix4x4 matT;
    matT = Matrix4x4::Translation(Vector3(0.0f, 0.0f, 5.0f));
    printf("[ View Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matV.m[i][j]);
        printf("\n");
    }
    printf("[ Translation Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matT.m[i][j]);
        printf("\n");
    }

    printf("\n=== Perspective Matrix Test ===\n");
    Matrix4x4 matP;
    matP = Matrix4x4::PerspectiveFovLH(ToRadian(90.0f), 1.0f, 1.0f, 10.0f);
    printf("[ Perspective Matrices ]\n");
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++)
            printf("%.2f ", matP.m[i][j]);
        printf("\n");
    }


    printf("\n=== Reflection Matrix Test ===\n");

    Vector3 floorNormal(0, 1, 0);
    float d = 0.0f;
    Matrix4x4 reflectMat = Matrix4x4::Reflection(floorNormal, d);

    Vector3 originalPos(0, 5, 0);

    Vector3 reflectedPos = Matrix4x4::TransformPoint(originalPos, reflectMat);

    printf("Original: %.1f, %.1f, %.1f\n", originalPos.x, originalPos.y, originalPos.z);
    printf("Reflected: %.1f, %.1f, %.1f\n", reflectedPos.x, reflectedPos.y, reflectedPos.z);

    // --- Canvas.h Test ---
    int width = 400;
    int height = 300;
    Canvas canvas(width, height);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            // Normalization
            float u = (float)x / (width - 1);
            float v = (float)y / (height - 1);

            Color color = { u, v, 0.2f};

            canvas.PutPixel(x, y, color);
        }
    }

    canvas.SaveToPPM("output.ppm");

*/ 
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
    DrawLine(canvas, screenPoints[0], screenPoints[1], Color::White());
    DrawLine(canvas, screenPoints[1], screenPoints[2], Color::White());
    DrawLine(canvas, screenPoints[2], screenPoints[0], Color::White());

    canvas.SaveToPPM("triangle.ppm");

    return 0;
}