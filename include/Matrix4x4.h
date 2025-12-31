#pragma once
#include "Vector3.h"

namespace Shika {

    struct alignas(16) Matrix4x4 {
       public : 
          union {
            // Two dimensional array m[row][col]
            float m[4][4];
            // for send data
            float e[16];
            // SIMD register
            __m128 row[4];
          };

       public : 
          // --- Basic Constructor ---
          Matrix4x4() {
            row[0] = _mm_setzero_ps();
            row[1] = _mm_setzero_ps();
            row[2] = _mm_setzero_ps();
            row[3] = _mm_setzero_ps();
          } 
       
       public : 
          // --- Static Helper ---
          // Return Identity Matrix
          static Matrix4x4 Identity() {
            Matrix4x4 mat;

            mat.row[0] = _mm_set_ps(0, 0, 0, 1);
            mat.row[1] = _mm_set_ps(0, 0, 1, 0);
            mat.row[2] = _mm_set_ps(0, 1, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);

            return mat;
          }

       private : 
          // Helper for Multiplication One Row
          inline __m128 MulRow(__m128 rowVec, const Matrix4x4 other) const{
            // Broadcast x, y, z, w
            __m128 x = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(0, 0, 0, 0));
            __m128 y = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(1, 1, 1, 1));
            __m128 z = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(2, 2, 2, 2));
            __m128 w = _mm_shuffle_ps(rowVec, rowVec, _MM_SHUFFLE(3, 3, 3, 3));
            
            __m128 r = _mm_mul_ps(x, other.row[0]);
            r = _mm_add_ps(r, _mm_mul_ps(y, other.row[1]));
            r = _mm_add_ps(r, _mm_mul_ps(z, other.row[2]));
            r = _mm_add_ps(r, _mm_mul_ps(w, other.row[3]));

            return r;
          }

       public : 
          // Matrix Multiplication Operator
          Matrix4x4 operator*(const Matrix4x4& other) const{
            Matrix4x4 result;
            result.row[0] = MulRow(row[0], other);
            result.row[1] = MulRow(row[1], other);
            result.row[2] = MulRow(row[2], other);
            result.row[3] = MulRow(row[3], other);
            return result;
          }

          // --- Transformaiton Matrices ---
          // Translation Matrices
          static Matrix4x4 Translation(const Vector3& translation) {
            Matrix4x4 mat = Identity();
            mat.row[3] = _mm_set_ps(1.0f, translation.z, translation.y, translation.x);
            return mat;
          }
          // Scaling Matricies
          static Matrix4x4 Scaling(const Vector3& scaling) {
            Matrix4x4 mat;
            mat.row[0] = _mm_set_ps(0, 0, 0, scaling.x);
            mat.row[1] = _mm_set_ps(0, 0, scaling.y, 0);
            mat.row[2] = _mm_set_ps(0, scaling.z, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around x-axis
          static Matrix4x4 RotationX(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, 0, 0, 1);
            mat.row[1] = _mm_set_ps(0, -s, c, 0);
            mat.row[2] = _mm_set_ps(0, c, s, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around y-axis
          static Matrix4x4 RotationY(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, s, 0, c);
            mat.row[1] = _mm_set_ps(0, 0, 1, 0);
            mat.row[2] = _mm_set_ps(0, c, 0, -s);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
          // Rotations around z-axis
          static Matrix4x4 RotationZ(float angleInRadian) {
            Matrix4x4 mat;
            float c = std::cos(angleInRadian);
            float s = std::sin(angleInRadian);
            mat.row[0] = _mm_set_ps(0, 0, -s, c);
            mat.row[1] = _mm_set_ps(0, 0, c, s);
            mat.row[2] = _mm_set_ps(0, 1, 0, 0);
            mat.row[3] = _mm_set_ps(1, 0, 0, 0);
            return mat;
          }
        
       public :
          // --- View&Projection ---
          // View Matrix (Left-Handed)
          static Matrix4x4 LookAtLH(const Vector3& eye, const Vector3& focus, const Vector3& up) {
            Vector3 f = (focus-eye).Normalized(); // Forward
            Vector3 r = up.Cross(f).Normalized(); // Right
            Vector3 u = f.Cross(r); // Up

            Matrix4x4 mat;
            // First Row (Right)
            mat.row[0] = _mm_set_ps(0, f.x, u.x, r.x);
            // Second Row (Up)
            mat.row[1] = _mm_set_ps(0, f.y, u.y, r.y);
            // Third Row (Forward)
            mat.row[2] = _mm_set_ps(0, f.z, u.z, r.z);
            // Fourth Row (Position)
            float valX = -r.Dot(eye);
            float valY = -u.Dot(eye);
            float valZ = -f.Dot(eye);
            mat.row[3] = _mm_set_ps(1, valZ, valY, valX);

            return mat;
          }
          // Projection Matrix (Left-Handed)
          // range : Z [0, 1] for DirectX
          static Matrix4x4 PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ) {
            Matrix4x4 mat;

            float rad = fovAngleY;
            float h = 1.0f / std::tan(rad / 2.0f); // Cotangent
            float w = h / aspectRatio;
            float q = farZ / (farZ - nearZ);

            mat.row[0] = _mm_set_ps(0, 0, 0, w);
            mat.row[1] = _mm_set_ps(0, 0, h, 0);
            mat.row[2] = _mm_set_ps(1, q, 0, 0);
            mat.row[3] = _mm_set_ps(0, -q * nearZ, 0, 0);

            return mat;
          }

    };
}