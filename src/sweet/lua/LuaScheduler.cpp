//
// LuaScheduler.cpp
// Copyright (c) 2013 Charles Baker.  All rights reserved.
//

#include "LuaScheduler.hpp"
#include "Lua.hpp"
#include "LuaThread.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet;
using namespace sweet::lua;

LuaScheduler::LuaScheduler( lua::Lua* lua )
: lua_( lua ),
  lua_threads_(),
  active_calls_( 0 )
{
    SWEET_ASSERT( lua_ );

    const int LUA_THREADS_RESERVE = 8;
    lua_threads_.reserve( LUA_THREADS_RESERVE );
    lua::LuaThread* lua_thread = new lua::LuaThread( *lua_ );
    lua_threads_.push_back( lua_thread );
}

LuaScheduler::~LuaScheduler()
{
    while ( !lua_threads_.empty() )
    {
        delete lua_threads_.back();
        lua_threads_.pop_back();
    }
}

void LuaScheduler::begin_call()
{
    lua_->error_policy().push_errors();
    ++active_calls_;
}

void LuaScheduler::end_call()
{
    SWEET_ASSERT( active_calls_ > 0 );
    --active_calls_;
    if ( lua_->error_policy().pop_errors() != 0 )
    {
        delete lua_threads_.back();
        lua_threads_.pop_back();
        lua::LuaThread* lua_thread = new lua::LuaThread( *lua_ );
        lua_threads_.push_back( lua_thread );
    }
}

void LuaScheduler::push_thread()
{
    lua::LuaThread* lua_thread = new lua::LuaThread( *lua_ );
    lua_threads_.push_back( lua_thread );
}

void LuaScheduler::pop_thread()
{
    SWEET_ASSERT( lua_threads_.size() > 1 );
    lua::LuaThread* lua_thread = lua_threads_.back();
    lua_threads_.pop_back();

    lua::LuaThread* resumed_lua_thread = lua_threads_.back();
    SWEET_ASSERT( resumed_lua_thread->get_state() == LUA_THREAD_SUSPENDED );
    begin_call();
    resumed_lua_thread->resume()
    .end();
    end_call();

    delete lua_thread;    
}

void LuaScheduler::pop_thread( lua_State* lua_state )
{
    SWEET_ASSERT( lua_threads_.size() > 1 );
    
    if ( lua_threads_.size() > 1 )
    {
        lua::LuaThread* lua_thread = lua_threads_.back();
        lua_threads_.pop_back();

        lua::LuaThread* resumed_lua_thread = lua_threads_.back();
        SWEET_ASSERT( resumed_lua_thread->get_state() == LUA_THREAD_SUSPENDED );
        begin_call();
        const int begin = 1;
        const int end = lua_gettop( lua_state ) + 1;
        resumed_lua_thread->resume()
            .copy_values_from_stack( lua_state, begin, end )
        .end();
        end_call();

        delete lua_thread;
    }
}

void LuaScheduler::destroy_lua_threads()
{
    while ( !lua_threads_.empty() )
    {
        delete lua_threads_.back();
        lua_threads_.pop_back();
    }
}

int LuaScheduler::lua_push_thread( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    const int RESULTS = 1;
    int results = int( luaL_optnumber(lua_state, RESULTS, 0) );
    LuaScheduler* scheduler = reinterpret_cast<LuaScheduler*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    scheduler->push_thread();
    return lua_yield( lua_state, results );
}

int LuaScheduler::lua_pop_thread( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );    
    LuaScheduler* scheduler = reinterpret_cast<LuaScheduler*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    scheduler->pop_thread( lua_state );
    return 0;
}

lua::LuaThread* LuaScheduler::lua_thread() const
{
    SWEET_ASSERT( !lua_threads_.empty() );
    return lua_threads_.back();
}

bool LuaScheduler::one_thread_only() const
{
    return lua_threads_.size() == 1;
}

int LuaScheduler::active_calls() const
{
    return active_calls_;
}
