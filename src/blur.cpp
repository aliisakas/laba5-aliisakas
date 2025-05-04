#include "blur.h"
#include <omp.h>

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



// Функция partBlur для размытия части строк
void partBlur(const Image& input, Image& output, int width, int height, int fromY, int toY) {

    for (int y = fromY; y < toY; ++y) {
        for (int x = 0; x < width; ++x) {
            output[y][x] = mid5x5(input, x, y, width, height);
        }
    }
}



// Функция parallellBlurThreads для параллельного размытия
Image parallellBlurThreads(const Image& input, int width, int height, int numThreads) {

    Image output(height, std::vector<Color>(width));

    std::vector<std::thread> threads;
    int rowsPerThread = height / numThreads;

    // Запускаем потоки
    for (int i = 0; i < numThreads; ++i) {
        
        int fromY = i * rowsPerThread;
        int toY = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread;

        // Создаём поток и добавляем его в вектор
        threads.emplace_back(partBlur, std::cref(input), std::ref(output),
                             width, height, fromY, toY);
    }

    // Дожидаемся завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    return output;
}



// Функция parallelBlurOpenMP для размытия с OpenMP
Image parallelBlurOpenMP(const Image& input, int width, int height) {
    Image output(height, std::vector<Color>(width));

    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            output[y][x] = mid5x5(input, x, y, width, height); 
        }
    }

    return output;
}