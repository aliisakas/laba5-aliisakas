#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>

const int numThreads = 8;
const int incNum = 1000;


void incMutex(int& counter, std::mutex& mtx) {
    for (int j = 0; j < incNum; ++j) {
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
    }
}

void mutexTest() {
    int counter = 0;
    std::mutex mtx;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;

    // Создаем потоки
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(incMutex, std::ref(counter), std::ref(mtx));
    }

    // Ждем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "MutexTest: counter = " << counter << ", time = " << duration.count() << " seconds" << std::endl;
}



void incAtomic(std::atomic<int>& counter) {
    for (int j = 0; j < incNum; ++j) {
        counter++;
    }
}

void atomicTest() {
    std::atomic<int> counter{0};

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;

    // Создаем потоки
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(incAtomic, std::ref(counter));
    }

    // Ждем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "AtomicTest: counter = " << counter << ", time = " << duration.count() << " seconds" << std::endl;
}

int main() {

    mutexTest(); 
    atomicTest(); 

    return 0;
}
