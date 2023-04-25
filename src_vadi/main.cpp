#include "matrix.h"
#include "randomnumgen.h"

int main() {
	randgen eng;

	matrix <int> mtrx(5, 5);
	for (size_t i = 0; i < 5; ++i) {
		for (size_t j = 0; j < 5; ++j) {
			mtrx[i][j] = eng.randint(0, 9);
		}
	}
	mtrx.print();
	for (size_t i = 0; i < 5; ++i) {
		std::cout << "Min value of col " << i << ": " << mtrx.col_min_value(i) << '\n';
	}
}
