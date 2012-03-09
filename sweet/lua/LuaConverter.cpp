//
// LuaConverter.cpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#include "LuaConverter.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Push a boolean value onto the Lua stack.
//
// @param lua
//  The lua_State to push the value onto the stack of.
//
// @param value
//  The value to push.
//
// @return
//  Nothing.
*/
void LuaConverter<bool>::push( lua_State* lua, bool value )
{
    SWEET_ASSERT( lua != 0 );
    lua_pushboolean( lua, value );
}

/**
// Get a bool value from the Lua stack.
//
// @param lua
//  The lua_State to get the function from.
//
// @param position
//  The position in the stack to get the value from.
//
// @return
//  The value.
*/
bool LuaConverter<bool>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state );
    if ( !lua_isboolean(lua_state, position) )
    {
        lua_pushfstring( lua_state, "The type '%s' is not a boolean as expected", lua_typename(lua_state, lua_type(lua_state, position)) );
        lua_error( lua_state );
    }
    return lua_toboolean( lua_state, position ) == 1 ? true : false;
}

/**
// Push a int value onto the Lua stack.
//
// @param lua
//  The lua_State to push the value onto the stack of.
//
// @param value
//  The value to push.
//
// @return
//  Nothing.
*/
void LuaConverter<int>::push( lua_State* lua, int value )
{
    SWEET_ASSERT( lua != 0 );
    lua_pushinteger( lua, static_cast<lua_Integer>(value) );
}

/**
// Get an int value from the Lua stack.
//
// @param lua
//  The lua_State to get the function from.
//
// @param position
//  The position in the stack to get the value from.
//
// @return
//  The value.
*/
int LuaConverter<int>::to( lua_State* lua, int position )
{
    SWEET_ASSERT( lua != 0 );
    if ( !lua_isnumber(lua, position) )
    {
        lua_pushfstring( lua, "The type '%s' is not an integer as expected", lua_typename(lua, lua_type(lua, position)) );
        lua_error( lua );
    }
    return lua_tointeger( lua, position );
}

/**
// Push a float value onto the Lua stack.
//
// @param lua
//  The lua_State to push the value onto the stack of.
//
// @param value
//  The value to push.
//
// @return
//  Nothing.
*/
void LuaConverter<float>::push( lua_State* lua, float value )
{
    SWEET_ASSERT( lua != 0 );
    lua_pushnumber( lua, static_cast<lua_Number>(value) );
}

/**
// Get a float value from the Lua stack.
//
// @param lua
//  The lua_State to get the function from.
//
// @param position
//  The position in the stack to get the value from.
//
// @return
//  The value.
*/
float LuaConverter<float>::to( lua_State* lua, int position )
{
    SWEET_ASSERT( lua != 0 );
    if ( !lua_isnumber(lua, position) )
    {
        lua_pushfstring( lua, "The type '%s' is not a float as expected", lua_typename(lua, lua_type(lua, position)) );
        lua_error( lua );
    }
    return static_cast<float>( lua_tonumber(lua, position) );
}

/**
// Push a string value onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the value onto the stack of.
//
// @param value
//  The value to push.
*/
void LuaConverter<std::string>::push( lua_State* lua_state, const std::string& value )
{
    SWEET_ASSERT( lua_state );
    lua_pushlstring( lua_state, value.c_str(), value.length() );
}

void LuaConverter<const LuaValue&>::push( lua_State* lua_state, const LuaValue& value )
{
    SWEET_ASSERT( lua_state );
    lua_pushlightuserdata( lua_state, const_cast<LuaValue*>(&value) );
    lua_gettable( lua_state, LUA_REGISTRYINDEX );
}

/**
// Get a string value from the Lua stack.
//
// @param lua
//  The lua_State to get the function from.
//
// @param position
//  The position in the stack to get the value from.
//
// @return
//  The value.
*/
std::string LuaConverter<std::string>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state );
    if ( !lua_isstring(lua_state, position) )
    {
        lua_pushfstring( lua_state, "The type '%s' is not a string as expected", lua_typename(lua_state, lua_type(lua_state, position)) );
        lua_error( lua_state );
    }
    return std::string( lua_tostring(lua_state, position) );
}

/**
// Push a string value onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the value onto the stack of.
//
// @param value
//  The value to push.
*/
void LuaConverter<const std::string&>::push( lua_State* lua_state, const std::string& value )
{
    SWEET_ASSERT( lua_state );
    lua_pushlstring( lua_state, value.c_str(), value.length() );
}
