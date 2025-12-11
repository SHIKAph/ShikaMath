#include <iostream>
#include <cstdio>
#include "../include/Vector3.h" 

using namespace Shika;

int main() {

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

    return 0;
}