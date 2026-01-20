#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <immintrin.h>

#ifdef _MSC_VER
    #define NO_INLINE __declspec(noinline)
#else
    #define NO_INLINE __attribute__((noinline))
#endif

// Simple Vector3 struct
struct alignas(16) Vector3 {
    __m128 v;

    Vector3() : v(_mm_setzero_ps()) {}
    Vector3(float x, float y, float z) : v(_mm_set_ps(0, z, y, x)) {}
    Vector3(__m128 _v) : v(_v) {}
};

// ---------------------------------------------------------
NO_INLINE Vector3 Normalize_Previous(Vector3 vec) {
    // 1. LengthSq (Dot self)
    __m128 dp = _mm_dp_ps(vec.v, vec.v, 0x7F);
    // 2. Sqrt 
    __m128 len = _mm_sqrt_ps(dp);
    // 3. Division 
    return Vector3(_mm_div_ps(vec.v, len));
}

NO_INLINE Vector3 Normalize_Current(Vector3 vec) {
    // 1. LengthSq
    __m128 dp = _mm_dp_ps(vec.v, vec.v, 0x7F);
    // 2. RSqrt
    __m128 rsqrt = _mm_rsqrt_ps(dp);
    // 3. Multiply 
    return Vector3(_mm_mul_ps(vec.v, rsqrt));
}
// ---------------------------------------------------------

int main() {
    const int ITERATIONS = 50'000'000; 
    
    Vector3 v1(10.0f, 20.0f, 30.0f);
    Vector3 v2(1.0f, 2.0f, 3.0f);
    
    volatile float prevent_opt = 0.0f; 

    std::cout << "=== Normalize Performance Test ===" << std::endl;

    // Now
    {
        auto start = std::chrono::high_resolution_clock::now();
        Vector3 res;
        for(int i=0; i<ITERATIONS; ++i) {
            res = Normalize_Previous(v1);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;
        std::cout << "Previous (sqrt + div) : " << ms.count() << " ms" << std::endl;
        
        float* f = (float*)&res; prevent_opt = f[0];
    }

    // Changed
    {
        auto start = std::chrono::high_resolution_clock::now();
        Vector3 res;
        for(int i=0; i<ITERATIONS; ++i) {
            res = Normalize_Current(v1);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;
        std::cout << "Current (rsqrt + mul)     : " << ms.count() << " ms" << std::endl;

        float* f = (float*)&res; prevent_opt = f[0];
    }


    return 0;
}