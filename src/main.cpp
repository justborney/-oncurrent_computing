#include <iostream>
#include <random>
#include <thread>
#include <vector>

#define MTRX_DIMENSIONS 8
#define PRNG_MIN 0
#define PRNG_MAX 9

//-------------------- random --------------------
typedef struct {
    std::mt19937 engine;
} PRNG;

void init_generator(PRNG &generator) {
    std::random_device device;
    generator.engine.seed(device());
}

int random(PRNG &generator, int min_val, int max_val) {
    std::uniform_int_distribution<int> distribution(min_val, max_val);
    return distribution(generator.engine);
}

//-------------------- matrix --------------------
std::vector<std::vector<int>> init_matrix(PRNG &generator, int size) {
    std::vector<std::vector<int>> matrix(size);

    for (size_t y = 0; y < size; ++y) {
        std::vector<int> temp(size);

        for (size_t x = 0; x < size; ++x) {
            temp[x] = random(generator, PRNG_MIN, PRNG_MAX);
        }
        matrix[y] = temp;
    }

    return matrix;
}

void print_vector(std::vector<int> &vec) {
    size_t i;
    for (i = 0; i < vec.size() - 1; ++i) {
        std::cout << vec[i] << ' ';
    }
    std::cout << vec[i] << '\n';
}

void print_matrix(std::vector<std::vector<int>> &matrix) {
    for (size_t y = 0; y < matrix.size(); ++y) {
        print_vector(matrix[y]);
    }
}

int min_value(std::vector<int> &vect) {
    int result = vect.at(0);
    for (size_t i = 1; i < vect.size(); ++i) {
        if (result > vect[i]) {
            result = vect[i];
        }
    }

    return result;
}

//-------------------- thread function --------------------
void thread_exec(std::vector<std::vector<int>> &matrix, int range_min, int range_max,
                 int &result) {
    size_t size = matrix[range_min].size();

    for (size_t i = range_min; i < range_max; ++i) {
        std::vector<int> column(size);
        for (size_t j = 0; j < size; ++j) {
            column[j] = matrix[j][i];
        }
        result += min_value(column);
    }
}

//-------------------- main --------------------
int main() {
    PRNG generator;
    init_generator(generator);

    std::vector<std::vector<int>> matrix = init_matrix(generator, MTRX_DIMENSIONS);

    int thread_count = 4;
    int vec_per_thread = MTRX_DIMENSIONS / thread_count;
    int vec_ind_start = 0;
    int vec_ind_end = vec_ind_start + vec_per_thread;

    std::vector<std::thread> threads;
    std::vector<int> min_vals(thread_count);

    for (size_t i = 0; i < thread_count; ++i) {
        std::thread temp(std::thread(thread_exec, std::ref(matrix), vec_ind_start,
                                     vec_ind_end, std::ref(min_vals.at(i))));
        threads.push_back(move(temp));

        vec_ind_end += vec_per_thread;
        vec_ind_start += vec_per_thread;
    }

    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    int result = 0;
    for (size_t i = 0; i < min_vals.size(); ++i) {
        result += min_vals[i];
    }

    std::cout << "Matrix:\n";
    print_matrix(matrix);
    std::cout << "Sum of minimal values: " << result << '\n';
}
