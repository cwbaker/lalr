//
// AddParameterHelper.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDPARAMETERHELPER_HPP_INCLUDED
#define SWEET_LUA_ADDPARAMETERHELPER_HPP_INCLUDED

#include "declspec.hpp"
#include "AddParameter.hpp"
#include "lua_/lua.h"
#include <sweet/traits/traits.hpp>

namespace sweet
{

namespace lua
{

class Lua;
class LuaValue;

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
        CALL_TYPE_NULL, ///< No call is in progress.
        CALL_TYPE_CALL, ///< A call is in progress.
        CALL_TYPE_RESUME ///< A resume is in progress.
    };

    lua_State* lua_state_; ///< The lua_State that this AddParameterHelper pushes parameters onto.
    Lua* lua_; ///< The Lua virtual machine that this AddParameterHelper is part of.
    int error_handler_; ///< The position on the stack that the error handler was pushed (or 0 if there is no error handler).
    int parameters_; ///< The number of parameters that have been pushed in the current call.
    CallType call_type_; ///< Whether the call is a call or a resume.

    public:
        AddParameterHelper( lua_State* lua_state, Lua* lua );

        AddParameter call( lua_Reader reader, void* context, const char* name );
        AddParameter call( const char* function );
        AddParameter call( const LuaValue& function );
        template <class Type> AddParameter call( const char* function, typename traits::traits<Type>::parameter_type object );

        AddParameter resume( lua_Reader reader, void* context, const char* name );
        AddParameter resume( const char* function );
        AddParameter resume( const LuaValue& function );
        template <class Type> AddParameter resume( const char* function, typename traits::traits<Type>::parameter_type object );
        AddParameter resume();

        void end();
        void end( bool* return_value );
        void end( int* return_value );
        void end( float* return_value );
        void end( std::string* return_value );
        void end( void** return_value );
        template <class Type> void end( Type* return_value );

        void push( const LuaNil& nil );
        void push( const LuaGlobalEnvironment& global_environment );
        void push( bool value );
        void push( int value );
        void push( float value );
        void push( const char* value );
        void push( const std::string& value );      
        void push( const LuaValue& value );
        void copy_values_from_stack( int begin, int end );

        void internal_begin( lua_Reader reader, void* context, const char* name );
        void internal_begin( const char* function );
        void internal_begin( const LuaValue& function );
        template <class Type> void internal_begin( const char* function, typename traits::traits<Type>::parameter_type object );

        void internal_end( int results );
        void internal_end_call( int parameters, int results, int error_handler );
        void internal_end_resume( int parameters );

        template <typename Type> void push( typename traits::traits<Type>::parameter_type value );

    private:
        static int stack_trace_for_call( lua_State* lua );
        static const char* stack_trace_for_resume( lua_State* lua_state, bool stack_trace_enabled, char* message, size_t length );
};

}

}

#endif
