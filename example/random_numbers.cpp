#include <cassert>
#include <iostream>
#include <random>

struct PRNG {
    std::mt19937 engine;
};

void init_generator(PRNG &generator) {
    std::random_device device;
    generator.engine.seed(device());
}

int random(PRNG &generator, int min_value, int max_value) {
    assert(min_value < max_value);
    std::uniform_int_distribution<int> distribution(min_value, max_value);
    return distribution(generator.engine);
}

double get_random_double(PRNG &generator, int min_value, int max_value) {
    assert(min_value < max_value);
    std::uniform_real_distribution<double> distribution(min_value, max_value);
    return distribution(generator.engine);
}

int main() {
    PRNG generator;
    init_generator(generator);
    std::cout << "10 int from [0,100]:\n";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << random(generator, 0, 100) << "\n";
    }

    std::cout << "10 double from [10,20]:\n";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << get_random_double(generator, 10.l, 20.l) << "\n";
    }
    return 0;
}
