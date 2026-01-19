#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h> // SIMD

// Simple Vector3 struct
struct alignas(16) Vector3 {
    __m128 v;

    Vector3() : v(_mm_setzero_ps()) {}
    Vector3(__m128 _v) : v(_v) {}
    Vector3(float x, float y, float z) : v(_mm_set_ps(0, z, y, x)) {}

    // Add operator
    Vector3 operator+(const Vector3& other) const {
        return Vector3(_mm_add_ps(v, other.v));
    }
};

// ---------------------------------------------------------

// 1. Call by Reference(const)
Vector3 Add_ByRef(const Vector3& a, const Vector3& b) {
    return a + b;
}

// 2. Call by Value 
Vector3 Add_ByVal(Vector3 a, Vector3 b) {
    return Vector3(_mm_add_ps(a.v, b.v));
}

// ---------------------------------------------------------

int main() {
    const int ITERATIONS = 100'000'000; 
    
    Vector3 v1(1.0f, 2.0f, 3.0f);
    Vector3 v2(4.0f, 5.0f, 6.0f);
    
    volatile float prevent_opt = 0.0f; 

    std::cout << "=== Vector3 Passing Benchmark (Iterations: " << ITERATIONS << ") ===\n\n";

    // 1. Reference Test
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        Vector3 result;
        for (int i = 0; i < ITERATIONS; ++i) {
            result = Add_ByRef(v1, v2); 
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        
        float* f = (float*)&result;
        prevent_opt = f[0]; 

        std::cout << "[1] Const Reference : " << elapsed.count() << " ms" << std::endl;
    }

    // 2. Value Test
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        Vector3 result;
        for (int i = 0; i < ITERATIONS; ++i) {
            result = Add_ByVal(v1, v2);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        
        float* f = (float*)&result;
        prevent_opt = f[0];

        std::cout << "[2] Call by Value   : " << elapsed.count() << " ms" << std::endl;
    }


    return 0;
}