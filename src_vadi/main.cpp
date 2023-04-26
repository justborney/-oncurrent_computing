#include <iomanip>
#include <thread>

#include "matrix.h"
#include "randomnumgen.h"
#include "threadwork.h"

#define ROW_TEST 15000
#define COL_TEST 15000

const std::vector<size_t> th_n = {1, 4, 8, 16};

void start_n_threads(size_t n, thread_stat &result);

int main(int argc, char *argv[]) {
    std::vector<thread_stat> stats(th_n.size());

    for (size_t i = 0; i < th_n.size(); ++i) {  // start & collect statistics
        start_n_threads(th_n[i], stats[i]);
    }

    for (size_t i = 0; i < th_n.size(); ++i) {  // prinout statistics
        std::cout << std::fixed << std::setprecision(2) << std::setw(12) << "Matrix "
                  << ROW_TEST << "x" << COL_TEST << "\n"
                  << "\tSumm of columns minimal values: " << stats[i].min_col_sum
                  << '\n'
                  << "\tNumber of threads: " << th_n[i] << '\n'
                  << "\tExecution time in secs: " << stats[i].exec_time.count() << '\n'
                  << "\tExecution time in millisecs: "
                  << milliseconds(stats[i].exec_time).count() << '\n'
                  << "\tExecution time in microsecs: "
                  << microseconds(stats[i].exec_time).count() << '\n';

        if (i == 0) {
            continue;
        }
        double speed_boost = stats[0].exec_time.count() / stats[i].exec_time.count();
        double effectiveness = speed_boost / th_n[i];
        std::cout << "\tSpeed boost: " << speed_boost << '\n'
                  << "\tEffectiveness: " << effectiveness << '\n';
    }
}

void start_n_threads(size_t n, thread_stat &result) {

    std::vector<std::thread> threads;
    std::vector<int> thread_sums(n);
    matrix<int> mtrx(ROW_TEST, COL_TEST);

    size_t col_step = COL_TEST / n;
    size_t col_start = 0;
    size_t col_end = col_step;

		const auto time_start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < n; ++i) {
        if (i == n - 1) {
            col_end = COL_TEST;
        }
        std::thread temp_thread(thread_func, std::ref(mtrx), col_start, col_end,
                                std::ref(thread_sums[i]));
        threads.push_back(move(temp_thread));

        col_start += col_step;
        col_end += col_step;
    }
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
		const auto time_finish = std::chrono::steady_clock::now();

    result.exec_time = time_finish - time_start;
    result.min_col_sum = thread_sums[0];
    for (size_t i = 1; i < thread_sums.size(); ++i) {
        result.min_col_sum += thread_sums[i];
    }
}
