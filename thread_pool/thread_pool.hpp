#pragma once

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

namespace xs
{

class thread_pool
{
  public:
    using size_type = std::size_t;
    using mutex_type = std::mutex;
    using lock_guard_type = std::lock_guard<mutex_type>;
    using unique_lock_type = std::unique_lock<mutex_type>;
    using task_type = std::function<void()>;

    thread_pool() : thread_pool(std::thread::hardware_concurrency())
    {
    }

    thread_pool(size_type _thread_num) : thread_num_(_thread_num)
    {
        size_type max_thread_num = std::min(_thread_num, static_cast<size_type>(std::thread::hardware_concurrency()));
        for (size_type i = 0U; i < max_thread_num; i++)
        {
            std::thread([this]() {
                task_type task;
                while (this->run_)
                {
                    this->get_task(task);
                    if (task != nullptr)
                    {
                        task();
                    }
                }
            }).detach();
        }
    }

    ~thread_pool()
    {
        {
            lock_guard_type guard(mtx_);
            run_ = false;
        }
        cv_.notify_all();
    }

    thread_pool(const thread_pool &) = delete;
    thread_pool &operator=(const thread_pool &) & = delete;
    thread_pool &operator=(thread_pool &&) && = delete;

    template <typename Func, typename... Args>
    auto submit(Func &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_type = typename std::result_of<Func(Args...)>::type;
        using packaged_task_type = std::packaged_task<return_type()>;

        auto exec = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
        auto task = std::make_shared<packaged_task_type>(std::move(exec));
        work_queue_.emplace([task]() { (*task)(); });
        cv_.notify_one();
        return task->get_future();
    }

    size_type thread_num()
    {
        return thread_num_;
    }

  private:
    void get_task(task_type &_task)
    {
        unique_lock_type lock(mtx_);
        if (work_queue_.empty())
        {
            cv_.wait(lock);
        }
        _task = std::move(work_queue_.front());
        work_queue_.pop();
    }

  private:
    bool run_ = true;
    mutex_type mtx_;
    size_type thread_num_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> work_queue_;
};

} // namespace xs

#endif