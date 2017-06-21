#include <chrono>
#include <iostream>
#include <margot.hpp>
#include <thread>

const int num_goals = 9;
const int change_goal_cycle = 2;
const int change_state_cycle = 5;
int goal_condition = 1;
bool state_condition = true;

void do_work(int trials, int some_knob, int value) {
  std::this_thread::sleep_for(std::chrono::milliseconds(trials * some_knob));
  std::cout << "Output: " << value << "\n";
}

void mock_configure() { std::cout << "** Called function **\n"; }

int main() {
  int trials = 100;
  int knob = 2;
  int repetitions = 30;
  int print = 0;
  for (int i = 0; i < repetitions; ++i) {
    std::cout << "\nExecution #" << i + 1 << "\n";
    if (i % change_goal_cycle == 0) {
      if (goal_condition >= num_goals) {
        goal_condition = 1;
      } else {
        goal_condition = goal_condition + 1;
      }
      std::cout << "** Changed goal condition value: " << goal_condition
                << " **\n";
    }
    if (i % change_state_cycle == 0) {
      state_condition = !state_condition;
      std::cout << "** Changed state **\n";
    }
    print = print + i;
    do_work(trials, knob, print);
  }
}
