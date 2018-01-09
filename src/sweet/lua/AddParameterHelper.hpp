#ifndef SWEET_LUA_ADDPARAMETERHELPER_HPP_INCLUDED
#define SWEET_LUA_ADDPARAMETERHELPER_HPP_INCLUDED

#include "declspec.hpp"
#include "AddParameter.hpp"
#include <lua/lua.hpp>
#include <sweet/traits/traits.hpp>

namespace sweet
{

namespace error
{

class ErrorPolicy;

}

namespace lua
{

class Lua;
class LuaValue;
class LuaThread;
class LuaThreadEventSink;

/**
// @internal
//
// Stores the state used by AddParameter to push parameters to function calls
// onto the Lua stack, call functions, and retrieve return values.
*/
class SWEET_LUA_DECLSPEC AddParameterHelper
{
    /**
    // The type of call that is currently being made by an AddParameterHelper.
    */
    enum CallType
    {
        CALL_TYPE_NULL, ///< No call is in progress or the initial call or resume failed.
        CALL_TYPE_CALL, ///< A call is in progress.
        CALL_TYPE_RESUME ///< A resume is in progress.
    };

    lua_State* lua_state_; ///< The lua_State that this AddParameterHelper pushes parameters onto.
    Lua* lua_; ///< The Lua virtual machine that this AddParameterHelper is part of.
    error::ErrorPolicy& error_policy_; ///< The ErrorPolicy that this AddParameterHelper reports errors to.
    int error_handler_; ///< The position on the stack that the error handler was pushed (or 0 if there is no error handler).
    int parameters_; ///< The number of parameters that have been pushed in the current call.
    CallType call_type_; ///< Whether the call is a call or a resume.
    LuaThreadEventSink* event_sink_; ///< The LuaThreadEventSink to fire returned and errored events at.
    void* context_; ///< Context to pass to LuaThreadEventSink event functions.

    public:
        AddParameterHelper( lua_State* lua_state, Lua* lua );

        void fire_returned( lua::LuaThread* thread );
        void fire_errored( lua::LuaThread* thread );
        void reset_event_sink_and_context();
        LuaThreadEventSink* event_sink() const;
        void* context() const;

        AddParameter call( lua_Reader reader, void* context, const char* name );
        AddParameter call( const char* function );
        AddParameter call( const LuaValue& function );
        AddParameter call( lua_State* lua_state, int position );
        template <class Type> AddParameter call( const char* function, typename traits::traits<Type>::parameter_type object );

        AddParameter resume( lua_Reader reader, void* context, const char* name );
        AddParameter resume( const char* function );
        AddParameter resume( const LuaValue& function );
        AddParameter resume( lua_State* lua_state, int position );
        template <class Type> AddParameter resume( const char* function, typename traits::traits<Type>::parameter_type object );
        AddParameter resume();

        void end();
        void end( bool* return_value );
        void end( int* return_value );
        void end( float* return_value );
        void end( std::string* return_value );
        void end( void** return_value );
        void end( LuaValue* return_value );
        void end( LuaThreadEventSink* event_sink, void* context );
        template <class Type> void end( Type* return_value );

        void push( const LuaNil& nil );
        void push( const LuaGlobalEnvironment& global_environment );
        void push( bool value );
        void push( int value );
        void push( float value );
        void push( const char* value );
        void push( const std::string& value );      
        void push( void* value );
        void push( const LuaValue& value );
        void copy_values_from_stack( int begin, int end );
        void copy_values_from_stack( lua_State* lua_state, int begin, int end );

        void internal_begin( lua_Reader reader, void* context, const char* name );
        void internal_begin( const char* function );
        void internal_begin( const LuaValue& function );
        void internal_begin( lua_State* lua_state, int position );
        template <class Type> void internal_begin( const char* function, typename traits::traits<Type>::parameter_type object );

        bool internal_end( int results );
        bool internal_end_call( int parameters, int results, int error_handler );
        bool internal_end_resume( int parameters );

        template <typename Type> void push( typename traits::traits<Type>::parameter_type value );
        template <typename Type> void push( typename traits::traits<Type>::parameter_type value, LuaObject* metatable );

    private:
        static int stack_trace_for_call( lua_State* lua );
        static const char* stack_trace_for_resume( lua_State* lua_state, bool stack_trace_enabled, char* message, int length );
};

}

}

#endif
