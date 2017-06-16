#include <chrono>
#include <iostream>
#include <margot.hpp>
#include <thread>

const int num_goals = 3;
const int change_goal_cycle = 4;
const int change_state_cycle = 5;

int goal_condition = 1;
bool state_condition = true;

void do_work(int trials) {
  std::this_thread::sleep_for(std::chrono::milliseconds(trials));
}

void mock_configure() { std::cout << "** Called function **\n"; }

int main() {
  int trials = 100;
  int repetitions = 20;
  for (int i = 0; i < repetitions; ++i) {
    std::cout << "\nExecution #" << i + 1 << "\n";
    if (i % change_goal_cycle == 0) {
      if (goal_condition >= num_goals) {
        goal_condition = 1;
      } else {
        goal_condition = goal_condition + 1;
      }
      std::cout << "** Changed my_execution_goal value **\n";
    }
    if (i % change_state_cycle == 0) {
      state_condition = !state_condition;
      std::cout << "** Changed state **\n";
    }
    do_work(trials);
  }
}
