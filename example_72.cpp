#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;

void hello(int number) {
  std::lock_guard<std::mutex> guard(mtx);
  std::cout << "Mem - " << number << "\n";
}

int main() {
  std::thread t1(hello, 1);
  std::thread t2(hello, 2);
  t1.join();
  t2.join();
}
