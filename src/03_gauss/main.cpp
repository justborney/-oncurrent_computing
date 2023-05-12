#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include <random>
#include <iomanip>

#define MTRX_DIMENSIONS 1000
#define PRNG_MIN 1.0
#define PRNG_MAX 9.0

using namespace std;
using seconds = chrono::duration<double>;
using milliseconds = chrono::duration<double, ratio_multiply<seconds::period, milli>>;
using microseconds = chrono::duration<double, ratio_multiply<seconds::period, micro>>;
using matrix = vector<vector<double>>;

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
std::vector<std::vector<double>> init_matrix(PRNG &generator, double size) {
    std::vector<std::vector<double>> matrix(size);

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

//------------------- gauss ---------------------
std::vector<double> gaussian_elimination(const std::vector<std::vector<double>>& matrix, const std::vector<double>& b, int num_threads) {
    const size_t n = matrix.size();
    int nt = num_threads;

    // Build augmented matrix
    std::vector<std::vector<double>> augmented_matrix(n, std::vector<double>(n + 1));
    for (size_t i = 0; i < n; ++i) {
        std::copy(matrix[i].begin(), matrix[i].end(), augmented_matrix[i].begin());
        augmented_matrix[i][n] = b[i];
    }

    // Gaussian elimination
    for (size_t k = 0; k < nt; ++k) {
        const double pivot = augmented_matrix[k][k];
        std::vector<std::thread> threads;

        for (size_t i = k + 1; i < nt; ++i) {
            const double factor = augmented_matrix[i][k] / pivot;

            threads.emplace_back([&, factor, i]() {
                for (size_t j = k; j <= nt; ++j) {
                    augmented_matrix[i][j] -= factor * augmented_matrix[k][j];
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    // Back substitution
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        const double sum = std::inner_product(augmented_matrix[i].begin() + i + 1, augmented_matrix[i].end() - 1, x.begin() + i + 1, 0.0);
        x[i] = (augmented_matrix[i][n] - sum) / augmented_matrix[i][i];
    }

    return x;
}

//-------------------- thread exec ------------------
void thread_exec(matrix &augmented_matrix; int range_min, int range_max, int iter) {
    double augmented_matrix_iter = augmented_matrix[iter][iter];
    for (size_t i = range_min; i < range_max; ++i) {
        if (augmented_matrix_iter * augmented_matrix[i][iter] < 0) {
            // for elems with different sign
            for (size_t j = 0; j < MTRX_DIMENSIONS; ++j) {
                if (augmented_matrix_iter < 0) {
                    augmented_matrix[i][j] += augmented_matrix[iter][j] / (-augmented_martix_iter) * augmented_matrix[i][iter];
                } else {
                    augmented_matrix[i][j] += augmented_matrix[iter][j] / augmented_matrix_iter * augmented_matrix[i];
                }
            }
        } else {
            //for elems with one sign
            for (size_t j = 0; j < MTRX_DIMENSIONS; ++j) {
                augmented_matrix[i][j] += augmented_matrix[iter][j] / augmented_matrix_iter * (-augmented_matrix[i][iter]);
            }
        }
    }

}

//-------------------- naive gauss ------------------
std::vector<double> naive_gaussian_elimination(const std::vector<std::vector<double>>& matrix, const std::vector<double>& b) {
    int n = matrix.size();

    std::vector<std::vector<double>> augmented_matrix(n);
    for (int i = 0; i < n; ++i) {
        augmented_matrix[i].resize(n+1);
        std::copy(matrix[i].begin(), matrix[i].end(), augmented_matrix[i].begin());
        augmented_matrix[i][n] = b[i];
    }

    for (int k = 0; k < n; ++k) {
        double pivot = augmented_matrix[k][k];
        for (int i = k + 1; i < n; ++i) {
            double factor = augmented_matrix[i][k] / pivot;

            for (int j = k; j <= n; ++j) {
                augmented_matrix[i][j] -= factor * augmented_matrix[k][j];
            }
        }
    }

    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = std::inner_product(augmented_matrix[i].begin() + i + 1, augmented_matrix[i].end() - 1, x.begin() + i + 1, 0.0);
        x[i] = (augmented_matrix[i][n] - sum) / augmented_matrix[i][i];
    }

    return x;
}

int main() {
    //std::vector<std::vector<double>> a = {{2.0, 1.0, -1.0}, {-3.0, -1.0, 2.0}, {-2.0, 1.0, 2.0}};
    //std::vector<double> b = {8.0, -11.0, -3.0};
    //std::vector<double> x = gaussian_elimination(a, b);

    //std::cout << "x = ";
    //for (double d : x) {
    //    std::cout << d << " ";
    //}
    
    PRNG generator;
    init_generator(generator);

    std::vector<std::vector<double>> matrix = init_matrix(generator, MTRX_DIMENSIONS);
    std::vector<double> b_vector = init_vector(generator, MTRX_DIMENSIONS);

    auto timed1 = chrono::steady_clock::now();
    auto timed2 = chrono::steady_clock::now();

    auto timed = timed2 - timed1;

    int threads_count[] = {1, 4, 8, 16};

    for (size_t i = 0; i < 4; ++i) {
        cout << "Number of threads: " << threads_count[i] << '\n';
        int thread_count = threads_count[i];

        const auto time_start = chrono::steady_clock::now();
        
        if (i == 0) {
            naive_gaussian_elimination(matrix, b_vector);
        } else {
            gaussian_elimination(matrix, b_vector, threads_count[i]);
        }

        const auto time_finish = chrono::steady_clock::now();
        const auto diff = time_finish - time_start;

        if (i == 0) timed = diff;
        else {
            cout << "speed up: " << seconds(timed).count() / seconds(diff).count() << '\n';
            cout << "effectiveness: " << seconds(timed).count() / seconds(diff).count() / threads_count[i] << '\n';
        }

        std::cout << fixed << std::setprecision(3) << std::setw(12) << seconds(diff).count()
                  << " sec,\n"
                  << setw(12) << milliseconds(diff).count() << " ms,\n"
                  << setw(12) << microseconds(diff).count() << " micros,\n\n";
    }
    return 0;
}
