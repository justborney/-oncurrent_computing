#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#define M_D 10
#define PRNG_MIN 1
#define PRNG_MAX 9

// matrix generation
// matrix being processed into blocks
// finding min column element and +=

// ---------- random ----------
typedef struct {
    std::mt19937 engine;
} PRNG;

void init_generator(PRNG &generator) {
    std::random_device device;
    generator.engine.seed(device());
}

int random(PRNG &generator, int min_value, int max_value) {
    std::uniform_real_distribution<double> distribution(min_value, max_value);
    return distribution(generator.engine);
}

// ---------- matrix ----------
int (*(matrix_generate)(PRNG &generator))[M_D] {
    static int matrix[M_D][M_D];

    for (size_t i = 0; i < M_D; ++i) {
        for (size_t j = 0; j < M_D; ++j) {
            matrix[i][j] = random(generator, PRNG_MIN, PRNG_MAX);
        }
    }

    return matrix;
}

// вынести отдельную функцию для подсчета мин элемента

// ---------- thread ----------
void thread_exec() {
    PRNG generator;
    init_generator(generator);
    int(*mtrx)[M_D];
    mtrx = matrix_generate(generator);

    int threads_count[] = {1, 4, 8, 16};
    int min_vals[M_D];

    for (size_t i = 0; i < 4; ++i) {
        int vec_per_thread = M_D / threads_count[i];
        int vec_ind_start = 0;
        int vec_ind_end = vec_ind_start + vec_per_thread;

        for (size_t j = 0; j < threads_count[i]; ++j) {
            std::thread temp(std::thread(thread_exec, mtrx, vec_ind_start, vec_ind_end,
                                         min_vals[j]));
        }
    }
}

int main() { return 0; }
