#ifndef RANDOMNUMGEN_H
#define RANDOMNUMGEN_H

#include <cassert>
#include <random>

class randgen {
   public:
    randgen() {
        std::random_device device;
        engine.seed(device());
    }

    ~randgen() {}

    int randint(int min_v, int max_v) {
        assert(min_v < max_v);
        std::uniform_int_distribution<int> distribution(min_v, max_v);
        return distribution(engine);
    }

    double randdouble(int min_v, int max_v) {
        assert(min_v < max_v);
        std::uniform_real_distribution<double> distribution(min_v, max_v);
        return distribution(engine);
    }

   private:
    std::mt19937 engine;
};

#endif  // RANDOMNUMGEN_H
