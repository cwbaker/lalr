//
// LuaRawWrapper.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUARAWWRAPPER_HPP_INCLUDED
#define SWEET_LUA_LUARAWWRAPPER_HPP_INCLUDED

#include "declspec.hpp"
#include "lua_/lua.h"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A wrapper that captures the address of a function and indicates that a 
// raw function should be generated for it instead of a standard function.
*/
class SWEET_LUA_DECLSPEC LuaRawWrapper
{
    lua_CFunction function_; ///< The address of the function to generate a yielding function for.

    public:
        LuaRawWrapper( lua_CFunction function );
        lua_CFunction get_function() const;
};

}

}

#endif

