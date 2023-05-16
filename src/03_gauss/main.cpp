#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#define MTRX_DIMENSIONS 100
#define PRNG_MIN 1.0
#define PRNG_MAX 9.0

using seconds = std::chrono::duration<double>;
using matrix_t = std::vector<std::vector<double>>;

//-------------------- random --------------------
typedef struct {
    std::mt19937 engine;
} PRNG;

void init_generator(PRNG &generator) {
    std::random_device device;
    generator.engine.seed(device());
}

double random(PRNG &generator, double min_val, double max_val) {
    std::uniform_real_distribution<double> distribution(min_val, max_val);
    return distribution(generator.engine);
}

//-------------------- matrix --------------------
matrix_t init_matrix(PRNG &generator, double size) {
    matrix_t matrix(size);

    for (size_t j = 0; j < size; ++j) {
        std::vector<double> temp(size);

        for (size_t i = 0; i < size; ++i) {
            temp[i] = random(generator, PRNG_MIN, PRNG_MAX);
        }
        matrix[j] = temp;
    }

    return matrix;
}

std::vector<double> init_vector(PRNG &generator, double size) {
    std::vector<double> my_vector(size);

    for (size_t i = 0; i < size; ++i) {
        my_vector[i] = random(generator, PRNG_MIN, PRNG_MAX);
    }

    return my_vector;
}

//------------------ augmented -----------------
matrix_t get_augmented_matrix(const matrix_t &matrix, const std::vector<double> &b) {
    matrix_t augmented_matrix(MTRX_DIMENSIONS);

    for (int i = 0; i < MTRX_DIMENSIONS; ++i) {
        augmented_matrix[i].resize(MTRX_DIMENSIONS + 1);
        std::copy(matrix[i].begin(), matrix[i].end(), augmented_matrix[i].begin());
        augmented_matrix[i][MTRX_DIMENSIONS] = b[i];
    }

    return augmented_matrix;
}

//-------------------- thread exec ------------------
void thread_exec(matrix_t &augmented_matrix, int range_min, int range_max, int iter) {
    double augmented_matrix_iter = augmented_matrix[iter][iter];

    for (size_t i = range_min; i < range_max; ++i) {
        if (augmented_matrix_iter * augmented_matrix[i][iter] < 0) {
            // for elems with different sign
            for (size_t j = 0; j < MTRX_DIMENSIONS + 1; ++j) {
                if (augmented_matrix_iter < 0) {
                    augmented_matrix[i][j] += augmented_matrix[iter][j] /
                                              (-augmented_matrix_iter) *
                                              augmented_matrix[i][iter];
                } else {
                    augmented_matrix[i][j] += augmented_matrix[iter][j] /
                                              augmented_matrix_iter *
                                              augmented_matrix[i][iter];
                }
            }
        } else {
            // for elems with one sign
            for (size_t j = 0; j < MTRX_DIMENSIONS + 1; ++j) {
                augmented_matrix[i][j] += augmented_matrix[iter][j] /
                                          augmented_matrix_iter *
                                          (-augmented_matrix[i][iter]);
            }
        }
    }
}

int main() {
    PRNG generator;
    init_generator(generator);

    matrix_t matrix = init_matrix(generator, MTRX_DIMENSIONS);
    std::vector<double> b_vector = init_vector(generator, MTRX_DIMENSIONS);
    matrix_t augm_matrix = get_augmented_matrix(matrix, b_vector);

    auto timed1 = std::chrono::steady_clock::now();
    auto timed2 = std::chrono::steady_clock::now();

    auto time_one_thread = timed2 - timed1;

    int threads_count[] = {1, 2, 4, 6, 8, 10, 12, 14, 16};

    for (size_t i = 0; i < std::end(threads_count) - std::begin(threads_count); ++i) {
        std::cout << "Number of threads: " << threads_count[i] << '\n';
        int thread_count = threads_count[i];
        int vec_per_thread = 0;
        int vec_ind_start = 0;
        int vec_ind_end = 0;

        const auto time_start = std::chrono::steady_clock::now();

        for (size_t j = 0; j < MTRX_DIMENSIONS - 1; ++j) {
            vec_per_thread = (MTRX_DIMENSIONS - 1 - j) / thread_count;
            vec_ind_start = j + 1;
            vec_ind_end = vec_ind_start + vec_per_thread;

            if (MTRX_DIMENSIONS - 1 - j < thread_count) --thread_count;

            std::vector<std::thread> threads;

            for (size_t k = 0; k < thread_count; ++k) {
                std::thread temp(std::thread(thread_exec, std::ref(augm_matrix),
                                             vec_ind_start, vec_ind_end, j));
                threads.push_back(move(temp));

                if (k != thread_count - 2) {
                    vec_ind_end += vec_per_thread;
                    vec_ind_start += vec_per_thread;
                } else {
                    vec_ind_start = vec_ind_end;
                    vec_ind_end = MTRX_DIMENSIONS;
                }
            }

            for (size_t th = 0; th < threads.size(); ++th) threads[th].join();
        }

        const auto time_finish = std::chrono::steady_clock::now();
        const auto diff = time_finish - time_start;

        std::cout << std::fixed << std::setprecision(3) << "Exec time:\t\t"
                  << seconds(diff).count() << " seconds\n";

        if (i == 0)
            time_one_thread = diff;
        else {
            std::cout << "Speed up:\t\t"
                      << seconds(time_one_thread).count() / seconds(diff).count()
                      << '\n';
            std::cout << "Effectiveness:\t\t"
                      << seconds(time_one_thread).count() / seconds(diff).count() /
                             threads_count[i]
                      << '\n';
        }

        std::cout << "----------------------------------------\n";
    }
    return 0;
}
