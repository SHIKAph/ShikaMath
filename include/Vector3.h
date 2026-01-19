#pragma once

#include "Common.h"

namespace Shika {

   struct alignas(16) Vector3 {
      public : 
         union {
            // Anonymous struct
            struct {float x, y, z;};
 
            // SIMD register
            __m128 v;

            // for indexing
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

      public : 
         // Operator Overloading
         // Add operator
         Vector3 operator+(Vector3 other) const {
            return Vector3( _mm_add_ps(v, other.v) );
         }
         // Sub operator
         Vector3 operator-(Vector3 other) const {
            return Vector3( _mm_sub_ps(v, other.v) );
         }
         // Scalar Multiplication operator (*Scalar)
         Vector3 operator*(float scalar) const {
            __m128 s = _mm_set1_ps(scalar);
            return Vector3( _mm_mul_ps(v, s) );
         }
         // Component-wise Multiplication operator (*Vector3)
         Vector3 operator*(Vector3 other) const {
            return Vector3( _mm_mul_ps(v, other.v) );
         }
 
         // Compound Assignment operator (*=Scalar)
         Vector3& operator*=(float scalar) {
            __m128 s = _mm_set1_ps(scalar);
            v = _mm_mul_ps(v, s);
            return *this;
         }

      public : 
         // Vector Geometry Functions
         // Dot Product
         float Dot(Vector3 other) const {
            __m128 mul = _mm_dp_ps(v, other.v, 0x7F);
            return _mm_cvtss_f32(mul);
         }

         // Length squared
         float LengthSq() const {
            return Dot(*this);
         }

         // Length / Magnitude
         float Length() const {
            return std::sqrt(LengthSq());
         }

         // Normalize
         void Normalize() {
            float len = Length();

            if (len < EPSILON) return;

            *this *= (1.0f / len);
         }
         // Return Normalized Vector3
         Vector3 Normalized() const {
            Vector3 temp = *this;
            temp.Normalize();
            return temp;
         }

         // Cross Product
         Vector3 Cross(Vector3 other) const {
            
            __m128 a_yzx = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 a_zxy = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2));
            __m128 b_yzx = _mm_shuffle_ps(other.v, other.v, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 b_zxy = _mm_shuffle_ps(other.v, other.v, _MM_SHUFFLE(3, 1, 0, 2));

            return Vector3( _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx) ) );
         }
   

   };

}