#ifndef SWEET_LUA_LUATHREADEVENTSINK_HPP_INCLUDED
#define SWEET_LUA_LUATHREADEVENTSINK_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{
    
namespace lua
{

class LuaThread;

class SWEET_LUA_DECLSPEC LuaThreadEventSink
{
    public:  
        virtual ~LuaThreadEventSink();
        virtual void lua_thread_returned( LuaThread* thread, void* context );
        virtual void lua_thread_errored( LuaThread* thread, void* context );
};
    
}

}

#endif
