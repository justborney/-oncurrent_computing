#include <iostream>
#include <algorithm>
#include <future>
#include <map>

static std::map<char, size_t> histogram(const std::string &s) {
    std::map<char, size_t> m;
    for (char c : s) {
        m[c] += 1;
    }

    return m;
}

static std::string sorted(std::string s) {
    std::sort(s.begin(), s.end());

    return s;
}

static bool is_vowel(char c) {
    std::string vowels = "aeiou";

    return vowels.end() != std::find(vowels.begin(), vowels.end(), c);
}

static size_t vowels(const std::string &s) {
    return std::count_if(s.begin(), s.end(), is_vowel);
}

int main() {
    std::string input;
    std::getline(std::cin, input);

    auto hist = std::async(std::launch::async, histogram, input);
    auto sorted_str = std::async(std::launch::async, sorted, input);
    auto vowel_count = std::async(std::launch::async, vowels, input);
    
    for (const auto &[c, count] : hist.get()) {
        std::cout << c << ": " << count << '\n';
    }

    std::cout << "Sorted string: " << sorted_str.get() << '\n' << "Vowels count: " << vowel_count.get();
}
