#ifndef SWEET_LUA_LUASCHEDULER_HPP_INCLUDED
#define SWEET_LUA_LUASCHEDULER_HPP_INCLUDED

#include <vector>

struct lua_State;

namespace sweet
{

namespace lua
{

class LuaThread;
class Lua;

class LuaScheduler
{
    lua::Lua* lua_;
    std::vector<lua::LuaThread*> lua_threads_;
    int active_calls_;

public:
    LuaScheduler( lua::Lua* lua );
    ~LuaScheduler();

    void begin_call();
    void end_call();

    void push_thread();
    void pop_thread();
    void pop_thread( lua_State* lua_state );
    void destroy_lua_threads();
    static int lua_push_thread( lua_State* lua_state );
    static int lua_pop_thread( lua_State* lua_state );
    lua::LuaThread* lua_thread() const;
    bool one_thread_only() const;
    int active_calls() const;
}; 

}

}

#endif
