//
// LuaThreadPool.cpp
// Copyright (c) 2014 Charles Baker.  All rights reserved.
//

#include "LuaThreadPool.hpp"
#include "LuaThreadPoolEventSink.hpp"
#include "LuaThread.hpp"
#include "Lua.hpp"
#include <sweet/assert/assert.hpp>
#include <algorithm>

using std::find;
using std::vector;
using namespace sweet;
using namespace sweet::lua;

/**
// Constructor.
*/
LuaThreadPool::LuaThreadPool()
: lua_( NULL ),
  event_sink_( NULL ),
  threads_(),
  free_threads_(),
  active_threads_()
{
}

/**
// Destructor.
*/
LuaThreadPool::~LuaThreadPool()
{
    destroy();
}

/**
// Get the LuaThread on which a call is currently being made.
//
// @return
//  The active LuaThread or null if there is no active LuaThread.
*/
lua::LuaThread* LuaThreadPool::active_thread() const
{
    return !active_threads_.empty() ? active_threads_.back() : NULL;
}

/**
// Create this LuaThreadPool.
//
// @param lua
//  The Lua virtual machine to use to create LuaThreads (assumed not null).
//
// @param reserve
//  The number of LuaThreads to initially create.
//
// @param event_sink
//  The LuaThreadPoolEventSink to fire events at when threads error or return
//  or null to ignore threads erroring or returning.
*/
void LuaThreadPool::create( lua::Lua* lua, unsigned int reserve, LuaThreadPoolEventSink* event_sink )
{
    SWEET_ASSERT( lua );

    destroy();
    lua_ = lua;
    event_sink_ = event_sink;
    threads_.reserve( reserve );
    free_threads_.reserve( reserve );
    active_threads_.reserve( reserve );
    for ( unsigned int i = 0; i < reserve; ++i )
    {
        LuaThread* thread = new LuaThread( *lua );
        thread->mobdebug_on();
        threads_.push_back( thread );
        free_threads_.push_back( thread );
    }
}

/**
// Destroy this LuaThreadPool.
*/
void LuaThreadPool::destroy()
{
    active_threads_.clear();
    free_threads_.clear();
    while ( !threads_.empty() )
    {
        delete threads_.back();
        threads_.pop_back();
    }
    lua_ = NULL;
    event_sink_ = NULL;
}

/**
// Retrieve a LuaThread from the free list allocating it if necessary.
//
// @return
//  The LuaThread.
*/
lua::LuaThread* LuaThreadPool::allocate_thread()
{
    if ( free_threads_.empty() )
    {
        LuaThread* thread = new LuaThread( *lua_ );
        thread->mobdebug_on();
        threads_.push_back( thread );
        free_threads_.push_back( thread );
    }

    LuaThread* thread = free_threads_.back();
    SWEET_ASSERT( lua_gettop(thread->get_lua_state()) == 0 );
    free_threads_.pop_back();
    return thread;
}

/**
// Release \e thread back to the free list.
//
// The LuaThread \e thread is assumed to have been previously returned from 
// this LuaThreadPool by a call to LuaThreadPool::allocate_thread().  If 
// \e thread has errored then it is destroyed rather than being returned to
// the free list as it is of no further use.
//
// @param thread
//  The LuaThread to release back to the free list (assumed to have been 
//  previously retrieved via LuaThreadPool::allocate_thread() from this 
//  LuaThreadPool).
*/
void LuaThreadPool::free_thread( lua::LuaThread* thread )
{
    if ( thread )
    {
        SWEET_ASSERT( find(threads_.begin(), threads_.end(), thread) != threads_.end() );
        SWEET_ASSERT( find(free_threads_.begin(), free_threads_.end(), thread) == free_threads_.end() );

        if ( thread->get_state() != LUA_THREAD_READY )
        {
            vector<LuaThread*>::iterator i = find( threads_.begin(), threads_.end(), thread );
            if ( i != threads_.end() )
            {
                threads_.erase( i );
                delete thread;
                thread = NULL;
            }
        }
        else
        {
            lua_settop( thread->get_lua_state(), 0 );
            free_threads_.push_back( thread );
        }
    }
}

/**
// Retrieve a LuaThread from the free list on which to make a call.
//
// The \e thread parameter is provided to allow the error checking logic 
// associated with beginning a call into Lua (pushing the ErrorPolicy's error
// count) can be carried out when resuming Lua coroutines that previously
// yielded and weren't returned to the thread pool.
//
// @param thread
//  The LuaThread to begin a call on or null to allocate a new LuaThread.
// 
// @return
//  A free LuaThread.
*/
lua::LuaThread* LuaThreadPool::begin_call( lua::LuaThread* thread )
{
    SWEET_ASSERT( lua_ );
    SWEET_ASSERT( !thread || find(threads_.begin(), threads_.end(), thread) != threads_.end() );
    SWEET_ASSERT( !thread || find(free_threads_.begin(), free_threads_.end(), thread) == free_threads_.end() );
    lua_->error_policy().push_errors();
    active_threads_.push_back( thread != NULL ? thread : allocate_thread() );
    return active_threads_.back();
}

/**
// Finish a call made on a LuaThread in this pool.
//
// If any errors have occured on this LuaThreadPool's Lua object or if the
// LuaThread is in the error state then \e thread is destroyed and null is
// returned.  The errored event is fired at this LuaThreadPool's event sink if
// it has one to allow any code that is expecting to be able to resume the
// coroutine later to recover gracefully.
//
// If \e thread is ready then it is returned to the free list and null is 
// returned.  This is intended to indicate that a new LuaThread should be 
// retrieved to make the next call.
//
// If \e thread is suspended then it is returned.  This is intended to 
// indicate that the LuaThread should be kept and used to resume the next
// appropriate call as the Lua script that was executing has yielded to
// await some result or event before being resumed.  The calling code is 
// expected to keep track of the LuaThread and resume it at an appropriate
// point later in time.
//
// @param thread
//  The LuaThread to finish a call on (assumed not null).
//
// @return
//  The LuaThread \e thread if it has been suspended and should be resumed 
//  later otherwise null if \e thread was ready or an error occured during 
//  execution.
*/
lua::LuaThread* LuaThreadPool::end_call( lua::LuaThread* thread )
{
    SWEET_ASSERT( thread );
    SWEET_ASSERT( !active_threads_.empty() );
    SWEET_ASSERT( active_threads_.back() == thread );
    SWEET_ASSERT( find(threads_.begin(), threads_.end(), thread) != threads_.end() );
    SWEET_ASSERT( find(free_threads_.begin(), free_threads_.end(), thread) == free_threads_.end() );

    if ( lua_->error_policy().pop_errors() != 0 || thread->get_state() == LUA_THREAD_ERROR )
    {
        thread->fire_errored();
        fire_errored( thread );
        free_thread( thread );
        thread = NULL;
    }
    else if ( thread->get_state() == LUA_THREAD_READY )
    {
        thread->fire_returned();
        fire_returned( thread );
        free_thread( thread );
        thread = NULL;
    }

    active_threads_.pop_back();
    return thread;
}

void LuaThreadPool::fire_errored( LuaThread* thread )
{
    if ( event_sink_ )
    {
        event_sink_->lua_thread_errored( this, thread );
    }
}

void LuaThreadPool::fire_returned( LuaThread* thread )
{
    if ( event_sink_ )
    {
        event_sink_->lua_thread_returned( this, thread );
    }
}
