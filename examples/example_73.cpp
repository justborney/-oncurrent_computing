#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int partial_sum1(std::vector<int>::iterator first, std::vector<int>::iterator last) {
    int result = 0;
    for (std::vector<int>::iterator it = first; it != last; ++it) {
        result += *it;
    }
    return result;
}

template <class Iterator>
auto partial_sum2(Iterator first, Iterator last) {
    typename std::iterator_traits<Iterator>::value_type result = 0.0;
    for (auto it = first; it != last; ++it) {
        result += *it;
    }
    return result;
}

int main() {
    std::vector<int> numbers = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    std::vector<double> f_numbers = {1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,
                                     8.0,  9.0,  10.0, 11.0, 12.0, 13.0, 14.0,
                                     15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
    int sum1 = 0, sum2 = 0;
    double sum3 = 0.0, sum4 = 0.0;
    auto mid = numbers.begin() + std::distance(numbers.begin(), numbers.end()) / 2;
    auto mid2 =
        f_numbers.begin() + std::distance(f_numbers.begin(), f_numbers.end()) / 2;
    std::thread th1(
        [&sum1, &numbers, &mid]() { sum1 = partial_sum1(numbers.begin(), mid); });
    std::thread th2(
        [&sum2, &numbers, &mid]() { sum2 = partial_sum1(mid, numbers.end()); });
    std::thread th3([&]() { sum3 = partial_sum2(f_numbers.begin(), mid2); });
    std::thread th4([&]() { sum4 = partial_sum2(mid2, f_numbers.end()); });
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    std::cout << "sum1 = " << sum1 << "\n";
    std::cout << "sum2 = " << sum2 << "\n";
    std::cout << "sum1 + sum2 = " << sum1 + sum2 << "\n\n";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "sum3 = " << sum3 << "\n";
    std::cout << "sum4 = " << sum4 << "\n";
    std::cout << "sum3 + sum4 = " << sum3 + sum4;
}
