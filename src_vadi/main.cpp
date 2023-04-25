#include "matrix.h"
#include "randomnumgen.h"

int main() {
	randgen eng;

	matrix <int> mtrx(5, 5);
	for (size_t i = 0; i < 5; ++i) {
		col_set_values_int(mtrx, i, eng, 0, 9);
	}
	mtrx.print();
	for (size_t i = 0; i < 5; ++i) {
		std::cout << "Min value of col " << i << ": " << mtrx.col_min_value(i) << '\n';
	}
}
