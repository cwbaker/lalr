//
// LuaYieldReturner.ipp
// Copyright (c) 2008  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUA_LUAYIELDRETURNER_IPP_INCLUDED
#define SWEET_LUA_LUA_LUAYIELDRETURNER_IPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// Get the value to return to Lua at the end of a yielding call from Lua into 
// C++.
//
// @param lua_state
//  The lua_State that a call is being made from
//
// @return
//  Returns the value returned by lua_yield().
*/
template <class ReturnType>
int LuaYieldReturner<ReturnType>::return_value( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    return lua_yield( lua_state, 1 );
}

}

}

#endif
