#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

#include "randomnumgen.h"

#define ROW_DEFAULT 8
#define COL_DEFAULT 8

template <typename T>
class matrix {
   public:
    matrix<T>() : row_n(ROW_DEFAULT), col_n(COL_DEFAULT) {
        mtrx.resize(row_n);
        for (size_t i = 0; i < row_n; ++i) {
            mtrx[i].resize(col_n);
        }
    }

    matrix<T>(size_t y, size_t x) : row_n(y), col_n(x) {
        assert(y > 0 && x > 0);
        mtrx.resize(row_n);
        for (size_t i = 0; i < row_n; ++i) {
            mtrx[i].resize(col_n);
        }
    }

    ~matrix<T>() {}

    size_t rows() { return row_n; }
    size_t cols() { return col_n; }

    std::vector<T>& operator[](size_t ind) { return mtrx[ind]; }

    T col_min_value(size_t ind) {
        T result = mtrx[0][ind];
        for (size_t i = 1; i < row_n; ++i) {
            if (result > mtrx[i][ind]) {
                result = mtrx[i][ind];
            }
        }

        return result;
    }

    void print() {
        for (size_t y = 0; y < row_n; ++y) {
            for (size_t x = 0; x < col_n; ++x) {
                std::cout << mtrx[y][x] << ' ';
            }
            std::cout << '\n';
        }
    }

   private:
    std::vector<std::vector<T> > mtrx;
    size_t row_n;
    size_t col_n;
};

#endif  // MATRIX_H
