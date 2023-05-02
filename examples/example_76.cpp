#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <execution>

class LogDuration {
public:
    LogDuration(const std::string &msg = " ") : message(msg + " : "), start(std::chrono::steady_clock::now()) {}
    ~LogDuration() {
        auto finish = std::chrono::steady_clock::now();
        auto duration = finish - start;
        std::cerr << message << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms." << '\n';
    }
private:
    std::string message;
    std::chrono::steady_clock::time_point start;
};

struct PRNG {
    std::mt19937 engine;
};

void init_generator(PRNG &generator) {
    std::random_device device;
    generator.engine.seed(device());
}

template<typename Iterator>
void make_vector(Iterator first, Iterator last) {
    PRNG generator;
    init_generator(generator);
    std::uniform_real_distribution<double> distribution(-10.f, 10.f);
    auto get_random_double = [=]() mutable { return distribution(generator.engine); };
    std::generate(std::execution::par, first, last, get_random_double);
}

template<typename Iterator>
void make_matrix(Iterator first, Iterator last) {
    int count = 0;
    //------------------------------------------
    
}

int main() {
    LogDuration all_time("Total time of exec");
    std::vector<double> numbers(19);
    std::vector<std::vector<int>> matrix(25, std::vector<int>(16));
    const int num_threads = 4;
    const int length = std::distance(numbers.begin(), numbers.end());
    const int block_size = length / num_threads;
    std::vector<std::thread> threads(num_threads);
    auto block_start = numbers.begin();
    for (size_t i = 0; i < (num_threads - 1); ++i) {
        auto block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread([=]() { make_vector(block_start, block_end); });
        block_start = block_end;
    }
    threads[num_threads - 1] = std::thread([&]() { make_vector(block_start, numbers.end()); });
    for (auto &entry : threads) {
        entry.join();
    }
    std::sort(numbers.begin(), numbers.end());
    for (const auto &item : numbers) {
        std::cout << std::setw(6) << std::setprecision(2) << std::fixed << item;
    }
    std::cout << "\n\n";

    const int page_length = std::distance(matrix.begin(), matrix.end()) / num_threads;
    std::vector<std::thread> threads_matrix(num_threads);
    auto page_start = matrix.begin();
    for (size_t i = 0; i < (num_threads - 1); ++i) {
        auto page_end = page_start;
        std::advance(page_end, page_length);
        threads_matrix[i] = std::thread([=]() { make_matrix(page_start, page_end); });
        page_start = page_end;
    }

    threads_matrix[num_threads - 1] = std::thread([&]() { make_matrix(page_start, matrix.end()); });
    for (auto &entry : threads_matrix) {
        entry.join();
    }
    for (auto it = matrix.begin(); it != matrix.end(); ++it) {
        for (auto it1 = (*it).begin(); it1 != (*it).end(); ++it1) {
            std::cout << std::setw(4) << *it1;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
