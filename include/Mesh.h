#pragma once

#include <vector>
#include "../include/Vector3.h"

namespace Shika {
    struct Mesh {
        std::vector<Vector3> vertices;
        std::vector<std::vector<int>> indices; 

        static Mesh CreateCube() {
            Mesh mesh;
            mesh.vertices = {
                {-1,  1, -1}, { 1,  1, -1}, {-1, -1, -1}, { 1, -1, -1},
                {-1,  1,  1}, { 1,  1,  1}, {-1, -1,  1}, { 1, -1,  1}
            };
            mesh.indices = {
                {0, 2, 1}, {2, 3, 1}, {1, 3, 5}, {3, 7, 5},
                {5, 7, 4}, {7, 6, 4}, {4, 6, 0}, {6, 2, 0},
                {4, 0, 5}, {0, 1, 5}, {2, 6, 3}, {6, 7, 3}
            };
            return mesh;
        }
    };
}