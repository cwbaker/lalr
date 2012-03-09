//
// LuaMemoryReader.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAMEMORYREADER_HPP_INCLUDED
#define SWEET_LUA_LUAMEMORYREADER_HPP_INCLUDED

#include "declspec.hpp"
#include "lua_/lua.h"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// An implementation of the lua_Reader function and its associated context
// that reads from memory.
*/
class SWEET_LUA_DECLSPEC LuaMemoryReader
{
    const char* first_; ///< The first character in memory to read from.
    const char* last_; ///< One past the last character in memory to read to.

    public:
        LuaMemoryReader( const char* first, const char* last );
        const char* read( size_t* size );
        static const char* reader( lua_State* lua, void* context, size_t* size );
};

}

}

#endif
