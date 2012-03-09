//
// LuaReturner.ipp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUA_LUARETURNER_IPP_INCLUDED
#define SWEET_LUA_LUA_LUARETURNER_IPP_INCLUDED

#include "LuaReturner.hpp"
#include "LuaReturnerPolicy.hpp"
#include "lua_/lua.h"

namespace sweet
{

namespace lua
{

inline int LuaReturner<void, LUA_POLICY_NULL>::return_( lua_State* lua_state )
{
    return 0;
}

inline int LuaReturner<void, LUA_POLICY_YIELD>::return_( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    return lua_yield( lua_state, 0 );
}

inline int LuaReturner<void, LUA_POLICY_YIELD | LUA_POLICY_WEAKEN>::return_( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    return lua_yield( lua_state, 0 );
}

template <class ReturnType>
int LuaReturner<ReturnType, LUA_POLICY_NULL>::return_( lua_State* lua_state, typename traits::traits<ReturnType>::parameter_type value )
{
    SWEET_ASSERT( lua_state );
    LuaConverter<ReturnType>::push( lua_state, value );
    return 1;
}

template <class ReturnType>
int LuaReturner<ReturnType, LUA_POLICY_YIELD>::return_( lua_State* lua_state, typename traits::traits<ReturnType>::parameter_type value )
{
    SWEET_ASSERT( lua_state );
    LuaConverter<ReturnType>::push( lua_state, value );
    return lua_yield( lua_state, 1 );
}

template <class ReturnType>
int LuaReturner<ReturnType, LUA_POLICY_WEAKEN>::return_( lua_State* lua_state, typename traits::traits<ReturnType>::parameter_type value )
{
    SWEET_ASSERT( lua_state );
    typedef traits::traits<ReturnType>::base_type base_type;
    LuaObjectConverter<ReturnType, LuaTraits<base_type>::storage_type>::weaken( lua_state, value );
    LuaObjectConverter<ReturnType, LuaTraits<base_type>::storage_type>::push( lua_state, value );
    return 1;
}

template <class ReturnType>
int LuaReturner<ReturnType, LUA_POLICY_YIELD | LUA_POLICY_WEAKEN>::return_( lua_State* lua_state, typename traits::traits<ReturnType>::parameter_type value )
{
    SWEET_ASSERT( lua_state );
    typedef traits::traits<ReturnType>::base_type base_type;
    LuaObjectConverter<ReturnType, LuaTraits<base_type>::storage_type>::weaken( lua_state, value );
    LuaObjectConverter<ReturnType, LuaTraits<base_type>::storage_type>::push( lua_state, value );
    return lua_yield( lua_state, 1 );
}

}

}

#endif
