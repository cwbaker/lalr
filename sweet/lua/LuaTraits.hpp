//
// LuaTraits.hpp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUATRAITS_HPP_INCLUDED
#define SWEET_LUA_LUATRAITS_HPP_INCLUDED

/**
// The macro to define a LuaTraits class that associates a C++ type with its
// Lua storage type - LuaByValue, LuaByReference, or LuaWeakReference.
*/
#define SWEET_LUA_TYPE_CONVERSION( type, storage ) namespace sweet { namespace lua { template <> struct LuaTraits<type> { typedef lua::storage storage_type; }; } } 

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A tag type used as a trait to specify that a type should be stored in the
// Lua virtual machine by value.
*/
struct LuaByValue {};

/**
// @internal
//
// A tag type used as a trait to specify that a type should be stored in the
// Lua virtual machine by reference.
*/
struct LuaByReference {};

/**
// @internal
//
// Extract trait information for Lua from types.
*/
template <class Type>
struct LuaTraits
{
    typedef LuaByValue storage_type;
};

class LuaObject;
template <>
struct LuaTraits<LuaObject>
{
    typedef LuaByReference storage_type;
};

}

}

#endif
