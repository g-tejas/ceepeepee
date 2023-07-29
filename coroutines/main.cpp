#include "scheduler.hpp"

#include "task.hpp"

#include <iostream>

Task TaskA(Scheduler& sch) {
    std::cout << "Starting TaskA\n";
    co_await std::suspend_always{};
    std::cout << "Executing TaskA\n";
    co_await std::suspend_always{};
    std::cout << "Finished TaskA\n";
}

Task TaskB(Scheduler& sch) {
    std::cout << "Starting TaskB\n";
    co_await std::suspend_always{};
    std::cout << "Executing TaskB\n";
    co_await std::suspend_always{};
    std::cout << "Finished TaskB\n";
}

int main() {
  Scheduler sch;

  sch.emplace(TaskA(sch).get_handle());
  sch.emplace(TaskB(sch).get_handle());

  std::cout << "Starting coroutine scheduler...\n";

  sch.schedule();
  sch.wait();
}
