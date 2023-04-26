#ifndef RANDOMNUMGEN_H
#define RANDOMNUMGEN_H

#include <cassert>
#include <random>

#define DEFAULT_MIN 0
#define DEFAULT_MAX 1000000

/* Motivation behind a prng class and a separate header for it:
 *		Hide this code, so there's no need to look at it, while I debug this
 *matrix shit Ability to pass random number generator as a single variable, get random
 *integer via class method
 */

class randgen {
   public:
    randgen() {
        std::random_device device;
        engine.seed(device());
    }

    randgen(int min_v, int max_v) : min(min_v), max(max_v) {
        assert(min < max);
        std::random_device device;
        engine.seed(device());
    }

    ~randgen() {}

    int randint() {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }

    double randdouble() {
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(engine);
    }

   private:
    std::mt19937 engine;
    int min = DEFAULT_MIN;
    int max = DEFAULT_MAX;
};

#endif  // RANDOMNUMGEN_H
