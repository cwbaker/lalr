//
// LuaReturnerPolicy.hpp
// Copyright (c) 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUARETURNERPOLICY_HPP_INCLUDED
#define SWEET_LUA_LUARETURNERPOLICY_HPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A mask that specifies policies to be matched by LuaReturner template
// specializations.
*/
enum LuaReturnerPolicy
{
    LUA_POLICY_NULL   = 0x00, // Return with standard conversion.
    LUA_POLICY_YIELD  = 0x02, // Yield.
    LUA_POLICY_WEAKEN = 0x04  // Weaken the returned value.
};    

}

}

#endif