//
// vector.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_SET_HPP_INCLUDED
#define SWEET_LUA_SET_HPP_INCLUDED

#include <set>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert std::sets that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the std::set itself onto the stack - the application
// is responsible for making sure that the std::set is allocated for at 
// least as long as the Lua iterator function is being used.
//
// @param lua_state
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The std::set to iterate over.
*/
template <class Key, class Compare, class Allocator>
struct LuaConverter<const std::set<Key, Compare, Allocator>&>
{
    static void push( lua_State* lua_state, const std::set<Key, Compare, Allocator>& value )
    {
        lua_push_iterator( lua_state, value.begin(), value.end() );
    }
};

/**
// @internal
//
// Convert std::multisets that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the std::multiset itself onto the stack - the application
// is responsible for making sure that the std::multiset is allocated for at 
// least as long as the Lua iterator function is being used.
//
// @param lua_state
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The std::multiset to iterate over.
*/
template <class Key, class Compare, class Allocator>
struct LuaConverter<const std::multiset<Key, Compare, Allocator>&>
{
    static void push( lua_State* lua_state, const std::multiset<Key, Compare, Allocator>& value )
    {
        lua_push_iterator( lua_state, value.begin(), value.end() );
    }
};

}

}

#endif
