//
// vector.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_HASH_SET_HPP_INCLUDED
#define SWEET_LUA_HASH_SET_HPP_INCLUDED

#include <hash_set>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert stdext::hash_sets that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the stdext::hash_set itself onto the stack - the application
// is responsible for making sure that the stdext::hash_set for at least as long
// as the Lua iterator function is being used.
//
// @param lua
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The stdext::hash_set to iterate over.
//
// @return
//  Nothing.
*/
template <class Key, class Traits, class Allocator>
struct LuaConverter<const stdext::hash_set<Key, Traits, Allocator>&>
{
    static void push( lua_State* lua, const stdext::hash_set<Key, Traits, Allocator>& value )
    {
        lua_push( lua, value.begin(), value.end() );
    }
};

}

}

#endif
