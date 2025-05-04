#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "third_party/stb_image.h"
#include "third_party/stb_image_write.h"
#include "include/blur.h"

#include <iostream>
#include <chrono>

int main() {

    int width, height, channels;
    unsigned char* data = stbi_load("C:\\Users\\Алиса\\source\\repos\\laba5-aliisakas\\input\\cat.jpg", &width, &height, &channels, 3);

    if (!data) {
        std::cerr << "Failed to load image. Error: " << stbi_failure_reason() << "\n";
        return 1;
    }

    std::cout << "Image loaded: " << width << "x" << height << std::endl;

    // Преобразуем в Image
    Image img(height, std::vector<Color>(width));
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3;
            img[y][x] = Color(data[i], data[i + 1], data[i + 2]);
        }

    // Освобождаем оригинальные данные
    stbi_image_free(data);

    // Измеряем время размытия
    auto start = std::chrono::high_resolution_clock::now();
    Image blurred = sequentialBlur(img, width, height);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Sequential blur took " << duration.count() << " seconds" << std::endl;

    // Сохраняем результат
    std::vector<unsigned char> newdata(width * height * 3);
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3;
            Color c = blurred[y][x];
            newdata[i] = c.r;
            newdata[i + 1] = c.g;
            newdata[i + 2] = c.b;
        }

    stbi_write_png("C:\\Users\\Алиса\\source\\repos\\laba5-aliisakas\\output\\blurredcat.png", width, height, 3, newdata.data(), width * 3);
    std::cout << "Output saved to output/blurredcat.png" << std::endl;

    return 0;
}