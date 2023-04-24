//
// ThreadPool.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ThreadPool.hpp"
#include "assert.hpp"
#include <stdlib.h>

using std::deque;
using std::mutex;
using std::thread;
using std::lock_guard;
using std::unique_lock;
using namespace lalr;

ThreadPool::ThreadPool()
: mutex_()
, jobs_empty_()
, jobs_ready_()
, jobs_()
, threads_()
, active_jobs_( 0 )
, running_( false )
{
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::start( int threads )
{
    LALR_ASSERT( threads > 0 );
    if ( !running_ )
    {
        unique_lock<mutex> lock( mutex_ );
        active_jobs_ = 0;
        running_ = true;
        threads_.reserve( threads );
        for ( int i = 0; i < threads; ++i )
        {
            threads_.push_back( thread(&ThreadPool::thread_process, this) );
        }
    }
}

void ThreadPool::stop()
{
    if ( !threads_.empty() )
    {
        {
            unique_lock<mutex> lock( mutex_ );
            if ( !jobs_.empty() )
            {
                jobs_empty_.wait( lock );
            }
            running_ = false;
            jobs_ready_.notify_all();
        }
        LALR_ASSERT( !running_ && jobs_.empty() );
        for ( thread& thread : threads_ )
        {
            thread.join();
        }
        threads_.clear();
    }
}

void ThreadPool::push_job( std::function<void()>&& job )
{
    {
        lock_guard<mutex> lock( mutex_ );
        jobs_.push_back( job );
        ++active_jobs_;
    }
    jobs_ready_.notify_one();
}

void ThreadPool::wait_idle()
{
    unique_lock<mutex> lock( mutex_ );
    while ( running_ && active_jobs_ > 0 )
    {
        jobs_empty_.wait( lock );
    }
}

void ThreadPool::thread_process()
{
    unique_lock<mutex> lock( mutex_ );
    while ( running_ )
    {
        while ( running_ && jobs_.empty() )
        {
            jobs_ready_.wait( lock );
        }

        while ( running_ && !jobs_.empty() )
        {
            std::function<void()> job = std::move( jobs_.front() );
            jobs_.pop_front();
            lock.unlock();
            job();
            lock.lock();
            --active_jobs_;
            if ( active_jobs_ <= 0 )
            {
                jobs_empty_.notify_all();
            }
        }
    }
}
