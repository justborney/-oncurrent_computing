#ifndef THREADWORK_H
#define THREADWORK_H

#include <chrono>

#include "matrix.h"
#include "randomnumgen.h"

using seconds = std::chrono::duration<double>;
using milliseconds =
    std::chrono::duration<double, std::ratio_multiply<seconds::period, std::milli>>;
using microseconds =
    std::chrono::duration<double, std::ratio_multiply<seconds::period, std::micro>>;

typedef struct {
    seconds exec_time;
    int min_col_sum;
} thread_stat;

/* Motivation behind this class:
 * 		The more correct way of calling it would have been "matrix slice"
 *			since you pass matrix pointer to it and 2 variables that define
 *			a range of this slice
 *		Create a thread, pass a function to it
 *		In the function create an instance of this class
 *		Call 'set_values' so matrix is filled with numbers
 *		Call 'calculate', which does all the work
 */

class thread_work {
   public:
    thread_work(matrix<int> &mtrx_v, size_t from_v, size_t to_v)
        : mtrx(&mtrx_v), ind_col_start(from_v), ind_col_end(to_v) {}

    ~thread_work() {}

    void set_values() {
        for (size_t y = 0; y < mtrx->rows(); ++y) {
            for (size_t x = ind_col_start; x < ind_col_end; ++x) {
                (*mtrx)[y][x] = gen.randint();  // (*mtrx) crap is because C++ (in C it
                                                // would've worked without it(probably))
            }
        }
    }

    void calculate() {  // only the search for summ of columns is timed
        const auto time_start = std::chrono::steady_clock::now();
        size_t result = mtrx->col_min_value(ind_col_start);
        for (size_t y = ind_col_start + 1; y < ind_col_end; ++y) {
            result += mtrx->col_min_value(y);
        }

        const auto time_finish = std::chrono::steady_clock::now();
        exec_time = time_finish - time_start;
        min_col_sum = result;
    }

    seconds get_exec_time() { return exec_time; }
    int get_min_col_sum() { return min_col_sum; }

   private:
    seconds exec_time;
    int min_col_sum;

    randgen gen;

    matrix<int> *mtrx = NULL;
    size_t ind_col_start;
    size_t ind_col_end;
};

/* As a means of returning calculated values from thread, a reference to 'thread_stat'
 * structure is used */
void thread_func(matrix<int> &mtrx, size_t from, size_t to, thread_stat &th_s) {
    thread_work th_w(mtrx, from, to);  // matrix slice is defined
    th_w.set_values();                 // random values to this slice is set
    th_w.calculate();                  // summ is calculated

    th_s.exec_time = th_w.get_exec_time();  // basically return values
    th_s.min_col_sum = th_w.get_min_col_sum();
}

#endif  // THREADWORK
