#include <thread>

#include "matrix.h"
#include "randomnumgen.h"
#include "threadwork.h"

int main() {
	matrix <int> mtrx(5, 5);
	thread_stat stat;

	std::thread th(thread_func, std::ref(mtrx), 0, 5, std::ref(stat));
	th.join();

	mtrx.print();
	std::cout << "Min values per column:\n";
	for (size_t i = 0; i < 5; ++i) {
		std::cout << mtrx.col_min_value(i) << ' ';
	} std::cout << '\n';

	std::cout << "Sum of min values: " << stat.min_col_sum << '\n';
	std::cout << "It took: " << stat.exec_time.count() << " seconds.\n";
	return 0;
}
