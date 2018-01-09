//
// LuaGlobalEnvironment.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAGLOBALENVIRONMENT_HPP_INCLUDED
#define SWEET_LUA_LUAGLOBALENVIRONMENT_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A type to represent the Lua global environment.
*/
class SWEET_LUA_DECLSPEC LuaGlobalEnvironment
{
public:
    LuaGlobalEnvironment();
};

SWEET_LUA_DECLSPEC extern const LuaGlobalEnvironment global_environment;

}

}

#endif
