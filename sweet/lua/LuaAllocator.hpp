//
// LuaAllocator.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAALLOCATOR_HPP_INCLUDED
#define SWEET_LUA_LUAALLOCATOR_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// An implementation of the lua_Alloc function and its associated context.
*/
class SWEET_LUA_DECLSPEC LuaAllocator
{
    public:
        static void* allocate( void* context, void* ptr, size_t osize, size_t nsize );
};

}

}

#endif
