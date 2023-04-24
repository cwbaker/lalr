#pragma once

#include <deque>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace lalr
{

/**
// A thread pool.
*/
class ThreadPool
{
    std::mutex mutex_;
    std::condition_variable jobs_empty_;
    std::condition_variable jobs_ready_;
    std::deque<std::function<void()>> jobs_;
    std::vector<std::thread> threads_;
    int active_jobs_;
    bool running_;

public:
    ThreadPool();
    ~ThreadPool();
    void start( int threads );
    void stop();
    void push_job( std::function<void()>&& job );
    void wait_idle();

private:
    void thread_process();
};

}
