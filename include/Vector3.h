#pragma once

#include "Common.h"

namespace Shika {

   struct alignas(16) Vector3 {
      public : 
         union {
            //Anonymous struct
            struct {float x, y, z;};
 
            //SIMD register
            __m128 v;

            //for indexing
            float e[4];
         };

      public : 
         // 1. Basic Constructor
         Vector3() 
         : v(_mm_setzero_ps()) {}
         // 2. Value Injection Constructor
         Vector3(float _x, float _y, float _z) 
         : v(_mm_set_ps(0.0f, _z, _y, _x)) {};
         // 3. SIMD Constructor
         Vector3(__m128 _v)
         : v(_v) {};
   };

}