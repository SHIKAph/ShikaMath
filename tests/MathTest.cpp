#include <iostream>
#include <cstdio>
#include "../include/Vector3.h" 
#include "../include/Matrix4x4.h" 

using namespace Shika;

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

*/
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
    

    return 0;
}