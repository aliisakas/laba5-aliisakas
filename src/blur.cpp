#include "blur.h"

// Функция mid5x5 для усреднения цвета из квадрата 5x5 пикселей вокруг точки (x,y)
Color mid5x5(const Image& img, int x, int y, int width, int height) {

    int r = 0, g = 0, b = 0;
    int counter = 0;

    for (int radY = -2; radY <= 2; ++radY) {
        for (int radX = -2; radX <= 2; ++radX) {

            int curX = x + radX;
            int curY = y + radY;

            if (curX >= 0 && curX < width && curY >= 0 && curY < height) {
                const Color& result = img[curY][curX];
                r += result.r;
                g += result.g;
                b += result.b;
                counter++;
            }
        }
    }

    return Color(static_cast<uint8_t>(r/counter), 
                 static_cast<uint8_t>(g/counter), 
                 static_cast<uint8_t>(b/counter));
}

Image sequentialBlur(const Image& input, int width, int height) {
    Image output(height, std::vector<Color>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            output[y][x] = mid5x5(input, x, y, width, height);
        }
    }
    return output;
}