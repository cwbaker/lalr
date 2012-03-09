//
// AddGlobal.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "AddGlobal.hpp"
#include "LuaStackGuard.hpp"
#include "LuaRawWrapper.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Constructor.
// 
// @param lua_state
//  The lua_State to set values in.
*/
AddGlobal::AddGlobal( lua_State* lua )
: lua_state_( lua )
{
    SWEET_ASSERT( lua_state_ );
}

/**
// Set a global variable to nil.
//
// @param name
//  The name of the variable to set to nil.
//
// @param nil
//  The LuaNil used to overload this function (ignored).
//
// @return
//  This AddGlobal.
*/
AddGlobal& AddGlobal::operator()( const char* name, const LuaNil& nil )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushnil( lua_state_ );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to the global environment.
//
// @param name
//  The name of the variable to set to the global environment.
//
// @param global_environment
//  The LuaGlobalEnvironment used to overload this function (ignored).
//
// @return
//  This AddGlobal.
*/
AddGlobal& AddGlobal::operator()( const char* name, const LuaGlobalEnvironment& global_environment )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushvalue( lua_state_, LUA_GLOBALSINDEX );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to a boolean value.
//
// @param name
//  The name of the variable to put the boolean value in.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, bool value )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushboolean( lua_state_, value ? 1 : 0 );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to an integer value.
//
// @param name
//  The name of the variable to put the integer value in.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, int value )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushnumber( lua_state_, static_cast<lua_Number>(value) );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to a floating point value.
//
// @param name
//  The name of the variable to put the float value in.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, float value )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushnumber( lua_state_, static_cast<lua_Number>(value) );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to a string value.
//
// @param name
//  The name of the variable to put the string value in.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, const char* value )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );
    SWEET_ASSERT( value );

    LuaStackGuard guard( lua_state_ );
    lua_pushstring( lua_state_, value );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to a string value.
//
// @param name
//  The name of the variable to put the string value in.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, const std::string& value )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushlstring( lua_state_, value.c_str(), value.length() );
    lua_setglobal( lua_state_, name );
    return *this;
}

/**
// Set a global variable to refer to a raw function with no up values.
//
// @param name
//  The name of the variable to put the object in.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @return
//  This AddGlobal object.
*/
AddGlobal& AddGlobal::operator()( const char* name, const LuaRawWrapper& raw_wrapper )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( name );

    LuaStackGuard guard( lua_state_ );
    lua_pushcclosure( lua_state_, raw_wrapper.get_function(), 0 );
    lua_setglobal( lua_state_, name );
    return *this;
}
