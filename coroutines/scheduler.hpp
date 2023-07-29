#ifndef CEEPEEPEE_SCHEDULER_HPP
#define CEEPEEPEE_SCHEDULER_HPP

#include <coroutine>
#include <list>
#include <queue>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

/**
 * @brief An implementation of a multi-threaded scheduler for coroutine scheduling
 */
class Scheduler {
public:
    Scheduler(size_t num_threads = std::thread::hardware_concurrency());

    void emplace(std::coroutine_handle<> task);
    void schedule();
    void wait();
private:
    void _enqueue(std::coroutine_handle<> task);
    void _process(std::coroutine_handle<> task);

    std::vector<std::thread> m_workers;
    std::vector<std::coroutine_handle<>> m_tasks;
    std::queue<std::coroutine_handle<>> m_pending_tasks;

    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop{false};
    std::atomic<size_t> m_finished{0};
};

#endif //CEEPEEPEE_SCHEDULER_HPP
