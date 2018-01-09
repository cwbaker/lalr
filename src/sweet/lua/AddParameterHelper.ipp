#ifndef SWEET_LUA_ADDPARAMETERHELPER_IPP_INCLUDED
#define SWEET_LUA_ADDPARAMETERHELPER_IPP_INCLUDED

#include "AddParameterHelper.hpp"
#include "LuaUserDataTemplate.hpp"
#include "LuaStackGuard.hpp"
#include "LuaConverter.hpp"
#include "Lua.hpp"
#include <sweet/error/ErrorPolicy.hpp>
#include <sweet/traits/traits.hpp>
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace lua
{

/**
// Call a member function in the main thread of this Lua virtual machine.
//
// @param function
//  The name of the function to call.
//
// @param object
//  The object to call the member function on.
//
// @return
//  An AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
template <class Type> 
AddParameter AddParameterHelper::call( const char* function, typename traits::traits<Type>::parameter_type object )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_CALL;
    internal_begin<Type>( function, object );
    return AddParameter( this );
}

/**
// Resume a member function.
//
// @param function
//  The name of the function to call.
//
// @param object
//  The object to call the member function on.
//
// @return
//  An AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
template <class Type>
AddParameter AddParameterHelper::resume( const char* function, typename traits::traits<Type>::parameter_type object )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_RESUME;
    internal_begin<Type>( function, object );
    return AddParameter( this );
}

/**
// Call the function and retrieve an arbitrary return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the variable to place the return value into (assumed not 
//  null).
*/
template <class Type> 
void AddParameterHelper::end( Type* return_value )
{
    SWEET_ASSERT( return_value );
    LuaStackGuard guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );
    internal_end( 1 );
    *return_value = LuaConverter<typename traits::traits<Type>::value_type>::to( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
}

/**
// Call a member function.
//
// @param function
//  The name of the function to call (assumed not null).
//
// @param object
//  The object to call the member function on.
*/
template <class Type>
void AddParameterHelper::internal_begin( const char* function, typename traits::traits<Type>::parameter_type object )
{
    SWEET_ASSERT( function );
    SWEET_ASSERT( lua_ ); 
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( error_handler_ == 0 );

//
// If stack traces are enabled then push the error handler function.
//
    if ( call_type_ == CALL_TYPE_CALL && lua_->is_stack_trace_enabled() )
    {
        lua_pushcfunction( lua_state_, &AddParameterHelper::stack_trace_for_call );
        error_handler_ = lua_gettop( lua_state_ );
    }
    else
    {
        error_handler_ = 0;
    }

//
// Push a value from the table corresponding to \e object as the function 
// to be called.
//
    LuaConverter<Type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, function );
    error_policy_.error( !lua_isfunction(lua_state_, -1) && !lua_istable(lua_state_, -1), "The member variable '%s' is not a function or a table", function );

//
// Remove the table that the member function was retrieved from from the
// Lua stack because it is not passed implicitly in the call.  The application
// code is free to pass whatever value it wants (including not passing 
// anything at all) for the 'self' parameter.
//
    lua_remove( lua_state_, -2 );
}

/**
// Push an arbitrary value onto the Lua stack.
//
// @param value
//  The value to push.
*/
template <typename Type> 
void AddParameterHelper::push( typename traits::traits<Type>::parameter_type value )
{
    LuaConverter<Type>::push( lua_state_, value );
    ++parameters_;
}

/**
// Push a value onto the Lua stack as userdata with a metatable.
//
// @param value
//  The value to push.
//
// @param metatable
//  The LuaObject for the Lua table to set as the value's metatable (assumed 
//  not null).
*/
template <typename Type> 
void AddParameterHelper::push( typename traits::traits<Type>::parameter_type value, LuaObject* metatable )
{
    SWEET_ASSERT( metatable );
    Type* new_value = reinterpret_cast<Type*>( lua_newuserdata(lua_state_, sizeof(lua::LuaUserDataTemplate<Type>)) );
    new (new_value) lua::LuaUserDataTemplate<Type>( SWEET_STATIC_TYPEID(Type), value );
    lua_push_object( lua_state_, metatable );
    lua_setmetatable( lua_state_, -2 );
    ++parameters_;
}

}

}

#endif
