#pragma once

#include <cmath> 
#include <cstdint>
#include <immintrin.h>


namespace Shika {
   //Define Constant Values & Helpers

   constexpr float PI = 3.1415926535f;
   constexpr float TwoPI = PI * 2.0f;
   constexpr float PiDiv2 = PI / 2.0f;

   constexpr float EPSILON = 1e-5f;

   constexpr inline float ToRadian(float deg) {
        return deg * (PI / 180.0f);
    }Feat: Initialize project with SIMD-aligned Vector3 and CMake setup

    constexpr inline float ToDegree(float rad) {
        return rad * (180.0f / PI);
    }

}