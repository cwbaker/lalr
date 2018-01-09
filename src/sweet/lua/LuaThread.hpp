#ifndef SWEET_LUA_LUATHREAD_HPP_INCLUDED
#define SWEET_LUA_LUATHREAD_HPP_INCLUDED

#include "declspec.hpp"
#include "AddParameterHelper.ipp"
#include "AddGlobal.ipp"
#include "AddParameter.ipp"
#include "LuaInvoker.ipp"
#include "LuaReturner.ipp"

struct lua_State;

namespace sweet
{

namespace lua
{

class Lua;
class LuaValue;
class LuaThreadEventSink;

/**
// @internal
//
// The state of a %Lua thread.
*/
enum LuaThreadState
{
    LUA_THREAD_READY, // The LuaThread is ready to execute.
    LUA_THREAD_SUSPENDED, // The LuaThread has yielded while executing code.
    LUA_THREAD_ERROR // The LuaThread has errored while executing code. 
};

/**
// A %Lua coroutine.
*/
class SWEET_LUA_DECLSPEC LuaThread
{
    Lua* lua_; ///< The Lua virtual machine that this LuaThread is part of.
    lua_State* lua_state_; ///< The lua_State that represents this LuaThread.
    AddParameterHelper add_parameter_helper_; ///< The AddParameterHelper that stores state for AddParameters.

    public:
        LuaThread( Lua& lua );
        ~LuaThread();

        Lua* get_lua() const;
        lua_State* get_lua_state() const;
        LuaThreadState get_state() const;
        LuaThreadEventSink* event_sink() const;
        void* context() const;

        void mobdebug_on();
        void mobdebug_off();
        void fire_returned();
        void fire_errored();
        void reset_event_sink_and_context();

        AddParameter call( lua_Reader reader, void* context, const char* name );
        AddParameter call( const char* filename, const char* name );
        AddParameter call( const char* first, const char* last, const char* name );
        AddParameter call( const char* function );
        AddParameter call( const LuaValue& function );
        AddParameter call( lua_State* lua_state, int position );
        template <class Type> AddParameter call( const char* function, const Type& object );

        AddParameter resume( const char* filename, const char* name );
        AddParameter resume( const char* first, const char* last, const char* name );
        AddParameter resume( const char* function );
        AddParameter resume( const LuaValue& function );
        AddParameter resume( lua_State* lua_state, int position );
        template <class Type> AddParameter resume( const char* function, const Type& object );
        AddParameter resume();
};

}

}

#include "LuaThread.ipp"

#endif
