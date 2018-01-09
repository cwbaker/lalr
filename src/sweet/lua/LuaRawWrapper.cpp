//
// LuaRawWrapper.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include "LuaRawWrapper.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Constructor.
//
// @param function
//  The address of the function that this LuaRawWrapper is wrapping.
*/
LuaRawWrapper::LuaRawWrapper( lua_CFunction function )
: function_( function )
{
    SWEET_ASSERT( function_ );
}

/**
// Get the address of the function that this LuaRawWrapper is wrapping.
//
// @return
//  The address of the function that this LuaRawWrapper is wrapping.
*/
lua_CFunction LuaRawWrapper::get_function() const
{
    SWEET_ASSERT( function_ );
    return function_;
}
