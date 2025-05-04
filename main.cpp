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
        std::cerr << "Failed to load image. Error: " << stbi_failure_reason() << std::endl;
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



    // Измеряем время последовательного размытия
    auto startSeq = std::chrono::high_resolution_clock::now();
    Image firstBlur = sequentialBlur(img, width, height);
    auto endSeq = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationSeq = endSeq - startSeq;
    std::cout << "Sequential blur took " << durationSeq.count() << " seconds" << std::endl;

    // Измеряем время параллельного размытия
    int numThreads = 5;
    auto startPar = std::chrono::high_resolution_clock::now();
    Image secondBlur = parallellBlurThreads(img, width, height, numThreads);
    auto endPar = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationPar = endPar - startPar;
    std::cout << "Parallell blur took " << durationPar.count() << " seconds" << std::endl;

    auto startOmp = std::chrono::high_resolution_clock::now();
    Image thirdBlur = parallelBlurOpenMP(img, width, height);
    auto endOmp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationOmp = endOmp - startOmp;
    std::cout << "OpenMP blur took: " << durationOmp.count() << " seconds" << std::endl;




    // Сохраняем результат послед. размытия
    std::vector<unsigned char> blurredcat(width * height * 3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3;
            Color c = firstBlur[y][x];
            blurredcat[i] = c.r;
            blurredcat[i + 1] = c.g;
            blurredcat[i + 2] = c.b;
        }
    }

    stbi_write_png("C:\\Users\\Алиса\\source\\repos\\laba5-aliisakas\\output\\blurredcat.png", width, height, 3, blurredcat.data(), width * 3);
    std::cout << "Output saved to output/blurredcat.png" << std::endl;

    // Сохраняем результат паралл. размытия
    std::vector<unsigned char> parblurredcat(width * height * 3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3;
            Color c = secondBlur[y][x];
            parblurredcat[i] = c.r;
            parblurredcat[i + 1] = c.g;
            parblurredcat[i + 2] = c.b;
        }
    }

    stbi_write_png("C:\\Users\\Алиса\\source\\repos\\laba5-aliisakas\\output\\parblurredcat.png", width, height, 3, parblurredcat.data(), width * 3);
    std::cout << "Output saved to output/parblurredcat.png" << std::endl;

    // Сохраняем результат размытия с OpenMP
    std::vector<unsigned char> OMPblurredcat(width * height * 3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3;
            Color c = thirdBlur[y][x];
            OMPblurredcat[i] = c.r;
            OMPblurredcat[i + 1] = c.g;
            OMPblurredcat[i + 2] = c.b;
        }
    }

    stbi_write_png("C:\\Users\\Алиса\\source\\repos\\laba5-aliisakas\\output\\OMPblurredcat.png", width, height, 3, OMPblurredcat.data(), width * 3);
    std::cout << "Output saved to output/OMPblurredcat.png" << std::endl;

    return 0;
}