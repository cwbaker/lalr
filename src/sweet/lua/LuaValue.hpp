//
// LuaValue.hpp
// Copyright (c) 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAVALUE_HPP_INCLUDED
#define SWEET_LUA_LUAVALUE_HPP_INCLUDED

#include "declspec.hpp"

struct lua_State;

namespace sweet
{

namespace lua
{

class Lua;

/**
// Hold a reference to a value in Lua so that it doesn't get garbage 
// collected.
//
// Stores a reference to the Lua value in the Lua registry using the address
// of the LuaValue as a key so that Lua considers the value referenced and 
// available for garbage collection.
*/
class SWEET_LUA_DECLSPEC LuaValue
{
    Lua* lua_;
    
public:
    LuaValue();
    LuaValue( Lua& lua, int position );
    LuaValue( Lua& lua, lua_State* lua_state, int position );
    LuaValue( const LuaValue& value );
    LuaValue& operator=( const LuaValue& value );
    ~LuaValue();
};

}

}

#endif