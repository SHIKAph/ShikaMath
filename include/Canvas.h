#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace Shika {

    struct Color {
        float r, g, b;

        // Basic Color Constant
        static const Color Black() {return {0, 0, 0}; } 
        static const Color White() {return {1, 1, 1}; }
        static const Color Red() {return {1, 0, 0}; }
        static const Color Green() {return {0, 1, 0}; }
        static const Color Blue() {return {0, 0, 1}; }

    };

    class Canvas {
        private: 
           int width;
           int height;
           std::vector<Color> pixels;
        
        public:
           Canvas(int w, int h) : width(w), height(h) {
               // Initialization (Black)
               pixels.resize(w * h, Color::Black());
           }

           // Put Pixel specific coordinates
           void PutPixel(int x, int y, const Color& color) {
               // Range Check
               if (x < 0 || x >= width || y <0 || y >= height) return;

               // 2-dimensional coordinate -> 1-dimensional Index
               pixels[y * width + x] = color;
           }

           // Remove All
           void Clear(const Color& color = Color::Black()) {
               std::fill(pixels.begin(), pixels.end(), color);
           }

           // Save PPM Format (.ppm)
           bool SaveToPPM(const std::string& filename) {
               std::ofstream ofs(filename);
               if (!ofs.is_open()) {
                   std::cerr << "Error: Could not open file " << filename << std::endl;
                   return false; 
               }

               // Write a header
               ofs << "P3\n" << width << " " << height << "\n255\n";

               // Write Pixel data
               for (const auto& p : pixels) {
                   // float(0.0~1.0) -> int (0~255) 
                   int ir = static_cast<int>(std::clamp(p.r, 0.0f, 1.0f) * 255.99f);
                   int ig = static_cast<int>(std::clamp(p.g, 0.0f, 1.0f) * 255.99f);
                   int ib = static_cast<int>(std::clamp(p.b, 0.0f, 1.0f) * 255.99f);
               
                   ofs << ir << " " << ig << " " << ib << "\n";
                }

                ofs.close();
                std::cout << "Image saved to" << filename << " (" << width << "x" << height << ")" << std::endl;
                return true;
           }

           int GetWidth() const {return width; }
           int Getheight() const {return height; }

    };
}