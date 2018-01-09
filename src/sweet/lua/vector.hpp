//
// vector.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_VECTOR_HPP_INCLUDED
#define SWEET_LUA_VECTOR_HPP_INCLUDED

#include "LuaConverter.hpp"
#include <vector>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert std::vectors that are pushed onto the Lua stack into Lua iterator
// functions.
//
// This doesn't push the std::vector itself onto the stack - the application
// is responsible for making sure that the std::vector is allocated for at 
// least as long as the Lua iterator function is being used.
//
// @param lua
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The std::vector to iterate over.
*/
template <class Type, class Allocator>
struct LuaConverter<const std::vector<Type, Allocator>&>
{
    static void push( lua_State* lua_state, const std::vector<Type, Allocator>& value )
    {
        SWEET_ASSERT( lua_state );
        lua_push_iterator( lua_state, value.begin(), value.end() );
    }
};

}

}

#endif
