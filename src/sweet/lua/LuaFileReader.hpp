//
// LuaFileReader.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAFILEREADER_HPP_INCLUDED
#define SWEET_LUA_LUAFILEREADER_HPP_INCLUDED

#include "declspec.hpp"
#include "lua_/lua.h"
#include <fstream>
#include <vector>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// An implementation of the lua_Reader function and its associated context
// that reads from a std::ifstream.
*/
class SWEET_LUA_DECLSPEC LuaFileReader
{
    std::ifstream       file_;
    int                 block_size_;
    std::vector<char>   block_;

    public:
        LuaFileReader( const char* filename, int block_size = 4096 );
        const char* read( size_t* size );
        static const char* reader( lua_State* lua, void* context, size_t* size );
};

}

}

#endif
