//
// LuaPosition.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAPOSITION_HPP_INCLUDED
#define SWEET_LUA_LUAPOSITION_HPP_INCLUDED

#include "LuaOutOfOrderParameter.hpp"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A class template that (with specializations) provides the index that a 
// parameter value can be found at on the Lua stack.
*/
template <class Type, int POSITION>
struct LuaPosition
{
    enum { position = lua_upvalueindex(POSITION) };
};

/**
// @internal
//
// Provide the index for an out of order parameter.
*/
template <int POSITION, int IGNORED_POSITION>
struct LuaPosition<LuaOutOfOrderParameter<POSITION>, IGNORED_POSITION>
{
    enum { position = POSITION };
};

}

}

#endif
