//
// LuaValue.cpp
// Copyright (c) 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "LuaValue.hpp"
#include "LuaConverter.hpp"
#include "Lua.hpp"

using namespace sweet::lua;

LuaValue::LuaValue()
: lua_( NULL )
{
}

LuaValue::LuaValue( Lua& lua, int position )
: lua_( &lua )
{
    SWEET_ASSERT( lua_ );
    lua_State* lua_state = lua_->get_lua_state();
    SWEET_ASSERT( lua_state );    
    lua_pushvalue( lua_state, position );
    lua_pushlightuserdata( lua_state, this );
    lua_insert( lua_state, -2 );
    lua_rawset( lua_state, LUA_REGISTRYINDEX );    
}

LuaValue::LuaValue( Lua& lua, lua_State* lua_state, int position )
: lua_( &lua )
{
    SWEET_ASSERT( lua_ );
    SWEET_ASSERT( lua_state );
    lua_pushvalue( lua_state, position );
    lua_pushlightuserdata( lua_state, this );
    lua_insert( lua_state, -2 );
    lua_rawset( lua_state, LUA_REGISTRYINDEX );    
}

LuaValue::LuaValue( const LuaValue& value )
: lua_( value.lua_ )
{
    if ( lua_ )
    {        
        lua_State* lua_state = lua_->get_lua_state();
        SWEET_ASSERT( lua_state );
        lua_pushlightuserdata( lua_state, this );
        lua_push( lua_state, value );
        lua_rawset( lua_state, LUA_REGISTRYINDEX );        
    }
}

LuaValue& LuaValue::operator=( const LuaValue& value )
{
    if ( this != &value )
    {
        if ( lua_ && lua_ != value.lua_ )
        {
            lua_State* lua_state = lua_->get_lua_state();
            SWEET_ASSERT( lua_state );
            lua_pushlightuserdata( lua_state, this );
            lua_pushnil( lua_state );
            lua_rawset( lua_state, LUA_REGISTRYINDEX );
        }
        
        lua_ = value.lua_;
        
        if ( lua_ )
        {
            lua_State* lua_state = lua_->get_lua_state();
            SWEET_ASSERT( lua_state );
            lua_pushlightuserdata( lua_state, this );
            lua_push( lua_state, value );
            lua_rawset( lua_state, LUA_REGISTRYINDEX );        
        }
    }
    return *this;
}

LuaValue::~LuaValue()
{
    if ( lua_ )
    {
        lua_State* lua_state = lua_->get_lua_state();
        SWEET_ASSERT( lua_state );
        lua_pushlightuserdata( lua_state, this );
        lua_pushnil( lua_state );
        lua_rawset( lua_state, LUA_REGISTRYINDEX );
        lua_ = NULL;
    }
}
