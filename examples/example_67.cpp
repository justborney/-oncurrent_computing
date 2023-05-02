#include <iostream>
#include <vector>
#include <future>
#include <numeric>

template<typename ContainerOfVectors>
ContainerOfVectors make_vector(int n) {
    ContainerOfVectors result(n);
    iota(result.begin(), result.end(), -4);

    return result;
}

int main() {
    std::vector<int> one = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<int> two = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int n = 10;

    // async sum of elems
    std::future<int> f1 = std::async([&one] { return accumulate(one.begin(), one.end(), 0); });
    std::future<int> f2 = std::async([&two] { return accumulate(two.begin(), two.end(), 0); });
    // async creation of an array with n elems
    std::future<std::vector<int>> f3 = async(std::launch::async, make_vector<std::vector<int>>, n);
    std::cout << f1.get() + f2.get() << '\n';
    
    for (auto item : f3.get()) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}
