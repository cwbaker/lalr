#ifndef SWEET_LUA_LUATHREADPOOL_HPP_INCLUDED
#define SWEET_LUA_LUATHREADPOOL_HPP_INCLUDED

#include "declspec.hpp"
#include <vector>

namespace sweet
{

namespace lua
{

class LuaThread;
class Lua;
class LuaThreadPoolEventSink;

class SWEET_LUA_DECLSPEC LuaThreadPool
{
    Lua* lua_;
    LuaThreadPoolEventSink* event_sink_;
    std::vector<lua::LuaThread*> threads_;
    std::vector<lua::LuaThread*> free_threads_;
    std::vector<lua::LuaThread*> active_threads_;

public:
    LuaThreadPool();
    ~LuaThreadPool();

    LuaThread* active_thread() const;

    void create( Lua* lua, unsigned int reserve, LuaThreadPoolEventSink* event_sink = 0 );
    void destroy();

    LuaThread* allocate_thread();
    void free_thread( LuaThread* thread );
    LuaThread* begin_call( LuaThread* thread = 0 );
    LuaThread* end_call( LuaThread* thread );

private:
    void fire_errored( LuaThread* thread );
    void fire_returned( LuaThread* thread );
}; 

}

}

#endif
