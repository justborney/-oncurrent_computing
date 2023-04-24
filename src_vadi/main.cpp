#include <iostream>
#include <cstddef>

// ------------------------------ globals & declarations
#define THREAD_N 4
size_t thread_n_arr[THREAD_N] = {1, 4, 8, 16};

bool str_isdigit(char *str);

void start(char **threads, size_t n);
void start(size_t threads[THREAD_N]);

// ------------------------------ main
int main(int argc, char *argv[]) {
	if (argc == 1) {
		start(thread_n_arr);
	} else {
		start(argv, argc);
	}
}

// ------------------------------ definitions
bool str_isdigit(char *str) {
	while (*str) {
		if (!(*str >= '0' && *str <= '9')) {
			return false;
		}
		++str;
	}
	return true;
}

void start(char **threads, size_t n) {
	for (size_t i = 1; i < n; ++i) {
		if (!(str_isdigit(threads[i]))) {
			std:: cout << "Invalid thread count cmd-argument: " << threads[i] << '\n';
		} else {
			// work(atoi(threads[i]));
			std::cout << "Working with thread count of " << threads[i] << '\n';
		}
	}
}

void start(size_t threads[THREAD_N]) {
	for (size_t i = 0; i < THREAD_N; ++i) {
		// work(threads[i]);
		std::cout << "Working with thread count of " << threads[i] << '\n';
	}
}

