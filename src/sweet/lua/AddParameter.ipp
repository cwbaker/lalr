//
// AddParameter.ipp
// Copyright (c) 2007 - 2014 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDPARAMETER_IPP_INCLUDED
#define SWEET_LUA_ADDPARAMETER_IPP_INCLUDED

#include "AddParameter.hpp"
#include "AddParameterHelper.hpp"

/**
// Push an arbitrary value onto the Lua stack.
//
// @param value
//  The value to push.
//
// @return
//  This AddParameter.
*/
template <typename Type> 
sweet::lua::AddParameter& sweet::lua::AddParameter::operator()( const Type& value )
{
    SWEET_ASSERT( add_parameter_helper_ );
    add_parameter_helper_->push<Type>( value );
    return *this;
}

/**
// Push a value onto the Lua stack as userdata with a metatable.
//
// @param value
//  The value to push onto the Lua stack as userdata.
//
// @param metatable
//  The LuaObject for the Lua table to set as the value's metatable (assumed 
//  not null).
*/
template <typename Type> 
sweet::lua::AddParameter& sweet::lua::AddParameter::operator()( const Type& value, LuaObject* metatable )
{
    SWEET_ASSERT( metatable );
    add_parameter_helper_->push<Type>( value, metatable );
    return *this;
}

/**
// Call the function and retrieve an arbitrary return value.
//
// This is done in this function instead of the destructor because calling 
// the function can throw an exception.
//
// @param return_value
//  A pointer to the variable to place the return value into (assumed not 
//  null).
*/
template <class Type> 
void sweet::lua::AddParameter::end( Type* return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end<Type>( return_value );
}

#endif
