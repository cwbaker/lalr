//
// AddParameter.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "AddParameter.hpp"
#include "AddParameterHelper.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Constructor.
//
// @param add_parameter_helper
//  The AddParameterHelper that is a member of the Lua or LuaThread object
//  that the call is being made on.
*/
AddParameter::AddParameter( AddParameterHelper* add_parameter_helper )
: add_parameter_helper_( add_parameter_helper )
{
    SWEET_ASSERT( add_parameter_helper_ );
}

/**
// Push a nil value onto the stack.
//
// @param nil
//  The %LuaNil type that is used to represent a %Lua nil value (ignored).
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( const LuaNil& nil )
{    
    add_parameter_helper_->push( nil );
    return *this;
}

/**
// Push the Lua global environment onto the stack.
//
// @param global_environment
//  The %LuaGlobalEnvironment type that is used to represent a %Lua 
//  global environment value (ignored).
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( const LuaGlobalEnvironment& global_environment )
{    
    add_parameter_helper_->push( global_environment );
    return *this;
}

/**
// Push the value referenced by \e value onto the stack.
//
// @param value
//  The %LuaValue that refers to the value to push onto the stack.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( const LuaValue& value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Push a boolean value onto the stack.
//
// @param value
//  The boolean value to push.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( bool value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Push an integer value onto the stack.
//
// @param value
//  The integer value to push.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( int value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Push a real value onto the stack.
//
// @param value
//  The real value to push.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( float value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Push a string value onto the stack.
//
// @param value
//  The string value to push.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( const char* value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Push a string value onto the stack.
//
// @param value
//  The string value to push.
//
// @return
//  This AddParameter.
*/
AddParameter& AddParameter::operator()( const std::string& value )
{
    add_parameter_helper_->push( value );
    return *this;
}

/**
// Copy the values on the stack in the range [\e begin, \e end) onto the top 
// of the stack to act as parameters to this call.
//
// Relative offsets represented by negative stack indices cannot be passed as
// the value of \e begin or \e end.  They must be positive values.
//
// @param begin
//  The position on the stack to start copying values from (assumed >= 1).
//
// @param end
//  One past the last position on the stack to copy values from 
//  (assumed >= \e begin).
*/
AddParameter& AddParameter::copy_values_from_stack( int begin, int end )
{
    add_parameter_helper_->copy_values_from_stack( begin, end );
    return *this;
}

/**
// Call the function.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @return
//  Nothing.
*/
void AddParameter::end()
{
    add_parameter_helper_->end();
}

/**
// Call the function and retrieve a boolean return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the boolean to place the return value into.
*/
void AddParameter::end( bool* return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end( return_value );
}

/**
// Call the function and retrieve an integer return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the int to place the return value into.
//
// @return
//  Nothing.
*/
void AddParameter::end( int* return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end( return_value );
}

/**
// Call the function and retrieve a real return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the float to place the return value into.
//
// @return
//  Nothing.
*/
void AddParameter::end( float* return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end( return_value );
}

/**
// Call the function and retrieve a string return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the string to place the return value into.
//
// @return
//  Nothing.
*/
void AddParameter::end( std::string* return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end( return_value );
}

/**
// Call the function and retrieve an object return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the void pointer to place the return value into.
//
// @return
//  Nothing.
*/
void AddParameter::end( void** return_value )
{
    SWEET_ASSERT( return_value );
    add_parameter_helper_->end( return_value );
}
