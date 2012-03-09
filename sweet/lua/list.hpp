//
// vector.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LIST_HPP_INCLUDED
#define SWEET_LUA_LIST_HPP_INCLUDED

#include <list>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert std::lists that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the std::list itself onto the stack - the application
// is responsible for making sure that the std::list for at least as long
// as the Lua iterator function is being used.
//
// @param lua_state
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The std::list to iterate over.
*/
template <class Type, class Allocator>
struct LuaConverter<const std::list<Type, Allocator>&>
{
    static void push( lua_State* lua_state, const std::list<Type, Allocator>& value )
    {
        lua_push_iterator( lua_state, value.begin(), value.end() );
    }
};

}

}

#endif
