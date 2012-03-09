//
// LuaMemoryReader.cpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#include "LuaMemoryReader.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Constructor.
//
// @param first
//  The first character in the script.
//
// @param last
//  One past the last character in the script.
*/
LuaMemoryReader::LuaMemoryReader( const char* first, const char* last )
: first_( first ),
  last_( last )
{
}

/**
// Read another block from this LuaMemoryReader's stream.
//
// @param size
//  A variable to receive the number of bytes in the block that is read
//  in.
//
// @return
//  A pointer to the begining of the block.
*/
const char* LuaMemoryReader::read( size_t* size )
{
    SWEET_ASSERT( size );
    *size = last_ - first_;

    const char* current = first_;
    first_ += (last_ - first_);
    return current;
}

/**
// @param lua
//  The lua_State.
//
// @param context
//  A pointer to the LuaMemoryReader that contains the context information for 
//  this call.
//
// @param size
//  A variable to receive the number of bytes in the block that is read
//  in.
//
// @return
//  A pointer to the begining of the block.
*/
const char* LuaMemoryReader::reader( lua_State* lua, void* context, size_t* size )
{
    SWEET_ASSERT( context );
    LuaMemoryReader* reader = reinterpret_cast<LuaMemoryReader*>( context );
    return reader->read( size );
}
