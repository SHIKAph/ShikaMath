#include <iostream>
#include <cstdio>
#include <cmath> 
#include "../include/Common.h"
#include "../include/Canvas.h"
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 
#include "Mesh.h"
#include "Rasterizer.h"

using namespace Shika;

int main() {

    const int width = 400;
    const int height = 300;
    Canvas canvas(width, height);

    Mesh cube = Mesh::CreateCube();

    // --- MVP Matrix ---
    Matrix4x4 matWorld = Matrix4x4::RotationX(ToRadian(30)) * Matrix4x4::RotationY(ToRadian(45)) * Matrix4x4::Translation({0, 0, 15});
    Matrix4x4 matView = Matrix4x4::LookAtLH({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    Matrix4x4 matProj = Matrix4x4::PerspectiveFovLH(ToRadian(50), (float)width/height, 0.1f, 100.0f);

    Matrix4x4 matMVP = matWorld * matView * matProj;

    // Directional Light
    Vector3 lightDir = Vector3(-1.0f, 1.0f, -1.0f).Normalized();

    // Render Loop
    for (const auto& tri : cube.indices) {

        // 1. Triangles Vertex
        Vector3 p0 = cube.vertices[tri[0]];
        Vector3 p1 = cube.vertices[tri[1]];
        Vector3 p2 = cube.vertices[tri[2]];

        // 2. Calculate FaceNormal
        Vector3 normal = Rasterizer::CalculateFaceNormal(p0, p1, p2);

        // 3. Normal -> World Space
        __m128 normalRotated = Matrix4x4::TransformDirection(normal, matWorld);
        Vector3 worldNormal = Vector3(normalRotated).Normalized();

        // Light Calculates with Lambert's Raw
        float intensity = std::max(0.0f, worldNormal.Dot(lightDir));
        intensity = std::clamp(intensity + 0.1f, 0.0f, 1.0f);

        // With Orange Color (R: 1.0, G: 0.5, B: 0.0)
        Color finalColor = {
            1.0f * intensity, 
            0.5f * intensity, 
            0.0f * intensity
        };

        Vector3 sp0 = Rasterizer::TransformVertex(p0, matMVP, width, height);
        Vector3 sp1 = Rasterizer::TransformVertex(p1, matMVP, width, height);
        Vector3 sp2 = Rasterizer::TransformVertex(p2, matMVP, width, height);

        Rasterizer::DrawFilledTriangle(canvas, sp0, sp1, sp2, finalColor);
    }


    canvas.SaveToPPM("Orange_shaded.cube.ppm");

    return 0;
}