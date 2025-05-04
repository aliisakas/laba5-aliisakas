#pragma once

#include <vector>
#include <cstdint>
#include <thread>

// Структура Color для хранения цвета пикселя в RGB
struct Color {
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
};

// Двумерный вектор пикселей Image для изображения
using Image = std::vector<std::vector<Color>>;


// Функции для размытия
Image sequentialBlur(const Image& input, int width, int height);

Image parallellBlurThreads(const Image& input, int width, int height, int numThreads);

Image parallelBlurOpenMP(const Image& input, int width, int height);