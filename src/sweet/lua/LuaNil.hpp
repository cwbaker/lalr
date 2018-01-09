//
// LuaNil.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUANIL_HPP_INCLUDED
#define SWEET_LUA_LUANIL_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A type to represent a Lua nil value.
*/
class SWEET_LUA_DECLSPEC LuaNil
{
public:
    LuaNil();
};

SWEET_LUA_DECLSPEC extern const LuaNil nil;

}

}

#endif
