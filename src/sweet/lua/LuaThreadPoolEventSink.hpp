#ifndef SWEET_LUA_LUATHREADPOOLEVENTSINK_HPP_INCLUDED
#define SWEET_LUA_LUATHREADPOOLEVENTSINK_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lua
{

class LuaThreadPool;
class LuaThread;

/**
// An interface to be implemented to respond to LuaThreadPool events.
*/
class SWEET_LUA_DECLSPEC LuaThreadPoolEventSink
{
public:
    virtual ~LuaThreadPoolEventSink();
    virtual void lua_thread_returned( LuaThreadPool* thread_pool, LuaThread* thread );
    virtual void lua_thread_errored( LuaThreadPool* thread_pool, LuaThread* thread );
};

}

}

#endif
