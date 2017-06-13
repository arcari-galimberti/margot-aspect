#include <chrono>
#include <margot.hpp>
#include <thread>

int condition = 1;

void do_work(int trials) {
  std::this_thread::sleep_for(std::chrono::milliseconds(trials));
}

int main() {
  int trials = 100;
  int repetitions = 10;
  for (int i = 0; i < repetitions; ++i) {
    condition = (i % 3) + 1;
    do_work(trials);
  }
}
