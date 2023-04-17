#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int partial_sum(std::vector<int>::iterator, std::vector<int>::iterator);

template <class Iterator>
auto partial_sum1(Iterator first, Iterator last) {
    typename std::iterator_traits<Iterator>::value_type result{};
    for (auto it = first; it != last; ++it) {
        result += *it;
    }
    return result;

    int main() {
        std::vector<int> numbers = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                    11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        std::vector<double> f_numbers = {1.0, 2.0, 3.0, 4.0, 5.0,
                                         6.0, 7.0, 8.0, 9.0, 10.0};
        int sum1 = 0, sum2 = 0;
        double sum3 = 0.0;
        auto mid = numbers.begin() + std::distance(numbers.begin(), numbers.end()) / 2;
        std::thread th1(
            [&sum1, &numbers, &mid]() { sum1 = partial_sum(numbers.begin(), mid); });
        std::thread th2(
            [&sum2, &numbers, &mid]() { sum2 = partial_sum(mid, numbers.end()); });
        std::thread th3([&]() {
            sum3 = partial_sum1<std::vector<double>::iterator, double>()(
                f_numbers.begin(), f_numbers.end());
        });
        th1.join();
        th2.join();
        th3.join();
        std::cout << "sum = " << sum3;
    }

    int partial_sum(std::vector<int>::iterator first, std::vector<int>::iterator last) {
        int result = 0;
        for (std::vector<int>::iterator it = first; it != last; ++it) {
            result += *it;
        }
        return result;
    }
