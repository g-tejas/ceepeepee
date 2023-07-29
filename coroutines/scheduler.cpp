#include "scheduler.hpp"

Scheduler::Scheduler(size_t num_threads) {
    m_workers.reserve(num_threads);

    for (size_t i = 0; i < num_threads; i++) {
        m_workers.emplace_back([this]() {
            while (true) {
                std::coroutine_handle<> task;
                {
                    std::unique_lock<std::mutex> lock(m_mtx);
                    m_cv.wait(lock, [this] {
                        return m_stop || (!m_pending_tasks.empty());
                    });

                    if (m_stop) {
                        return;
                    }

                    task = m_pending_tasks.front();
                    m_pending_tasks.pop();
                }

                if (task) {
                    _process(task);
                }
            }
        });
    }
}

void Scheduler::emplace(std::coroutine_handle<> task) {
    m_tasks.emplace_back(task);
}

/**
 * @brief Loops through the tasks and puts them into the pending_tasks
 */
void Scheduler::schedule() {
    for (auto task : m_tasks) {
        _enqueue(task);
    }
}

void Scheduler::wait() {
    for (auto& worker : m_workers) {
        worker.join();
    }
}

void Scheduler::_process(std::coroutine_handle<> task) {
    task.resume();

    if (!task.done()) {
        _enqueue(task);
    } else {
        task.destroy();
        if (m_finished.fetch_add(1) + 1 == m_tasks.size()) {
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_stop = true;
            }
            m_cv.notify_all();
        }
    }
}

void Scheduler::_enqueue(std::coroutine_handle<> task) {
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_pending_tasks.push(task);
    }
    m_cv.notify_one();
}

