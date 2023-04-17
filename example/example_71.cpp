#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;

void print_1(char ch) {
    // Вариант без синхронизации
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            std::cout << ch;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void print_2(char ch) {
    // Вариант с синхронизацией
    mtx.lock(); // Начало блока
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 10; j++) {
            std::cout << ch;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    mtx.unlock(); // Конец блока
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Выполняется параллельно с каждым потоком
}

void print_3(char ch) {{
    // Другой вариант с синхронизацией
    std::lock_guard<std::mutex> guard(mtx);
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            std::cout << ch;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Выполняется параллельно с каждым потоком
}

int main() {
    std::cout << "Запуск варианта без синхронизации\n";
    std::thread th1(print_1, '#');
    std::thread th2(print_1, '*');
    th1.join();
    th2.join();

    std::cout << "Запуск варианта с синхронизацией\n";
    std::thread th3(print_2, '#');
    std::thread th4(print_2, '*');
    th3.join();
    th4.join();

    std::cout << "Запуск другого варианта с синхронизацией\n";
    std::thread th5(print_3, '$');
    std::thread th6(print_3, '%');
    th5.join();
    th6.join();    
}
