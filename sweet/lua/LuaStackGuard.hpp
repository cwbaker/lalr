//
// LuaStackGuard.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUASTACKGUARD_HPP_INCLUDED
#define SWEET_LUA_LUASTACKGUARD_HPP_INCLUDED

#include "declspec.hpp"
#include "lua_/lua.h"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A guard that ensures that the top level stack for a Lua state is restored
// when a scope is left even if an exception is thrown.
*/
class SWEET_LUA_DECLSPEC LuaStackGuard
{
    lua_State* lua_state_; ///< The Lua state to save and restore the top of the stack of.
    int restore_to_position_; ///< The position to restore the top of the stack to on destruction.

    public:
        LuaStackGuard( lua_State* lua_state, int restore_to_position_delta = 0 );
        ~LuaStackGuard();
        void reset_to_top_of_stack( int restore_to_position_delta = 0 );
};

}

}

#endif
