//
// lua_functions.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_FUNCTIONS_HPP_INCLUDED
#define SWEET_LUA_FUNCTIONS_HPP_INCLUDED

#include "declspec.hpp"
#include "LuaRawWrapper.hpp"
#include "LuaValueWrapper.hpp"
#include "LuaPolicyWrapper.hpp"
#include "LuaReturnerPolicy.hpp"
#include "lua_/lua.h"
#include <sweet/traits/traits.hpp>
#include <string>

namespace sweet
{

namespace rtti
{

class Type;

}

namespace lua
{

class LuaValue;

SWEET_LUA_DECLSPEC void lua_dump_stack( lua_State* lua_state );
SWEET_LUA_DECLSPEC void lua_validate_type( lua_State* lua, int position, const rtti::Type& type );

SWEET_LUA_DECLSPEC void lua_push( lua_State* lua_state, bool value );
SWEET_LUA_DECLSPEC void lua_push( lua_State* lua_state, int value );
SWEET_LUA_DECLSPEC void lua_push( lua_State* lua_state, float value );
SWEET_LUA_DECLSPEC void lua_push( lua_State* lua_state, const std::string& value );
SWEET_LUA_DECLSPEC void lua_push( lua_State* lua_state, const LuaValue& value );
template <class Type> void lua_push( lua_State* lua_state, Type* value );
template <class Type> void lua_push( lua_State* lua_state, const Type* value );

SWEET_LUA_DECLSPEC void lua_create_object( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void lua_create_object_with_existing_table( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void lua_destroy_object( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void lua_weaken_object( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void lua_strengthen_object( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void lua_push_object( lua_State* lua_state, void* object );
SWEET_LUA_DECLSPEC void* lua_to_object( lua_State* lua_state, int position, const rtti::Type& type );
template <class Type> void lua_push_value( lua_State* lua_state, typename traits::traits<Type>::parameter_type value );
template <class Type> typename traits::traits<Type>::reference_type lua_to_value( lua_State* lua_state, int position );

SWEET_LUA_DECLSPEC LuaRawWrapper raw( lua_CFunction function );
template <class Type> LuaValueWrapper<Type> value( Type value );
template <class Function> LuaPolicyWrapper<Function, LUA_POLICY_YIELD> yield( Function function );
template <class Function> LuaPolicyWrapper<Function, LUA_POLICY_WEAKEN> weaken( Function function );

template <class Type> int lua_gc( lua_State* lua );
template <class Iterator> int lua_iterator( lua_State* lua );
template <class Iterator, class Function> int lua_iterator_with_function( lua_State* lua );
template <class Iterator> void lua_push_iterator( lua_State* lua, Iterator start, Iterator finish );
template <class Iterator, class Function> void lua_push_iterator( lua_State* lua, Iterator start, Iterator finish, const Function& function );

}

}

#endif
