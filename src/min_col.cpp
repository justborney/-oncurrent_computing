#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

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

int main() {
    PRNG generator;
    init_generator(generator);
    int (*mtrx)[M_D];
    mtrx = matrix_generate(generator);
    
    for (size_t i = 0; i < M_D; ++i) {
        for (size_t j = 0; j < M_D; ++j) {
            std::cout << mtrx[i][j] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
