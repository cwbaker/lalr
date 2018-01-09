//
// hash_map.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_HASH_MAP_HPP_INCLUDED
#define SWEET_LUA_HASH_MAP_HPP_INCLUDED

#include <hash_map>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert stdext::hash_maps that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the stdext::hash_map itself onto the stack - the application
// is responsible for making sure that the stdext::hash_map is allocated for at 
// least as long as the Lua iterator function is being used.
//
// @param lua_state
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The stdext::hash_map to iterate over.
*/
template <class Key, class Data, class Traits, class Allocator>
struct LuaConverter<const stdext::hash_map<Key, Data, Traits, Allocator>&>
{
    static void push( lua_State* lua_state, const stdext::hash_map<Key, Data, Traits, Allocator>& value )
    {
        lua_push( lua_state, value.begin(), value.end() );
    }
};

}

}

#endif
