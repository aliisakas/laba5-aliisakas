#pragma once

#include <vector>
#include <cstdint>

// Структура Color для хранения цвета пикселя в RGB
struct Color {
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
};

// Двумерный вектор пикселей Image для изображения
using Image = std::vector<std::vector<Color>>;

// Функция sequentialBlur для последовательного размытия 3x3
Image sequentialBlur(const Image& input, int width, int height);