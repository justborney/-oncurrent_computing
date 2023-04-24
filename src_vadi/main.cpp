#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <iomanip>

// ------------------------------ globals & declarations & inline funcs
// ********** chrono
using seconds = std::chrono::duration <double>;
using milliseconds = std::chrono::duration <double, std::ratio_multiply <seconds::period, std::milli>>;
using microseconds = std::chrono::duration <double, std::ratio_multiply <seconds::period, std::micro>>;

// ********** random
typedef struct {
	std::mt19937 engine;
} PRNG;

void init_rand(PRNG &gen) {
	std::random_device device;
	gen.engine.seed(device());
}

int random(PRNG &gen, int min_val, int max_val) {
	std::uniform_int_distribution <int> distribution(min_val, max_val);
	return distribution(gen.engine);
}

#define MIN_RAND 0
#define MAX_RAND 9

// ********** thread info & matrix info
#define THREAD_N 4
size_t thread_n[THREAD_N] = {1, 4, 8, 16};

#define Y 1600
#define X 1600

// ********** other
typedef struct {
	seconds exec_time;
	size_t sum;
} thread_stat;

void one_thread(thread_stat &th_stat);
void n_thread(thread_start &th_start, size_t thread_amount);

// ------------------------------ main
int main() {
	thread_stat th_stats[THREAD_N];
	one_thread(th_stats[0]);

}

// ------------------------------ definitions
void one_thread(thread_stat &th_stat) {
	const auto time_start = std::chrono::steady_clock::now();

	PRNG eng;
	init_rand(eng);
	int matrix[Y][X];
	for (size_t y = 0; y < Y; ++y) {
		for (size_t x = 0; x < X; ++x) {
			matrix[y][x] = random(eng, MIN_RAND, MAX_RAND);
		}
	}
	
	size_t min_col_sum = 0;
	size_t min_col_sums[X];
	for (size_t i = 0; i < X; ++i) {
		min_col_sums[i] = matrix[0][i];
	}

	for (size_t y = 1; y < Y; ++y) {
		for (size_t x = 0; x < X; ++x) {
			if (min_col_sums[x] > matrix[y][x]) {
				min_col_sums[x] = matrix[y][x];
			}
		}
	}

	for (size_t i = 0; i < X; ++i) {
		min_col_sum += min_col_sums[i];
	}
	const auto time_finish = std::chrono::steady_clock::now();
	const auto diff = time_start - time_finish;

	th_stat.exec_time = diff;
	th_stat.sum = min_col_sum;
}

void n_thread(thread_start &th_start, size_t thread_amount) {
	const auto time_start = std::chrono::steady_clock::now();

	int matrix[Y][X];
	size_t col_per_thread = thread_amount / X;

	const auto time_finish = std::chrono::steady_clock::now();
	
}

