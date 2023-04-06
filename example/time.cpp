#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using seconds = chrono::duration<double>;
using milliseconds = chrono::duration<double, ratio_multiply<seconds::period, milli>>;
using microseconds = chrono::duration<double, ratio_multiply<seconds::period, micro>>;

static pair<string, seconds> get_input() {
    string s;
    const auto start = chrono::steady_clock::now();
    if (!(cin >> s)) {
        return { {}, {}};
    }
    const auto finish = chrono::steady_clock::now();
    return { s, finish - start };
}

int main() {
    while (true) {
        cout << "Enter \"C++\"\n";
        const auto [user_input, diff] = get_input();
        if (user_input == "") {
            break;
        }
        if (user_input == "C++") {
            cout << "You did it\n" << fixed << setprecision(2) << setw(12) << diff.count() << " sec,\n"
                << setw(12) << milliseconds(diff).count() << " ms,\n"
                << setw(12) << microseconds(diff).count() << " micros,\n";
            break;
        } else {
            cout << "Invalid input.\n";
        }
    }
}
