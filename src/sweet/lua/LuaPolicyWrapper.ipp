//
// LuaPolicyWrapper.ipp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAPOLICYWRAPPER_IPP_INCLUDED
#define SWEET_LUA_LUAPOLICYWRAPPER_IPP_INCLUDED

#include "LuaPolicyWrapper.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace lua
{

/**
// Constructor.
//
// @param function
//  The address of the function that this LuaPolicyWrapper is wrapping.
*/
template <class Function, int POLICY>
LuaPolicyWrapper<Function, POLICY>::LuaPolicyWrapper( Function function )
: function_( function )
{
    SWEET_ASSERT( function_ );
}

/**
// Get the address of the function that this LuaPolicyWrapper is wrapping.
//
// @return
//  The address of the function that this LuaPolicyWrapper is wrapping.
*/
template <class Function, int POLICY>
Function LuaPolicyWrapper<Function, POLICY>::get_function() const
{
    SWEET_ASSERT( function_  );
    return function_;
}

}

}

#endif
