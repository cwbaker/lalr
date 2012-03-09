//
// LuaStackGuard.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "LuaStackGuard.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

/**
// Constructor.
//
// @param lua_state
//  The lua_State to save and restore the stack top of.
//
// @param restore_to_position_delta
//  The number of stack entries less than the current top of the stack to 
//  restore the top of the stack to when this LuaStackGuard is destroyed.
*/
LuaStackGuard::LuaStackGuard( lua_State* lua_state, int restore_to_position_delta )
: lua_state_( lua_state ),
  restore_to_position_( 0 )
{
    SWEET_ASSERT( lua_state_ );
    reset_to_top_of_stack( restore_to_position_delta );
}

/**
// Destructor.
*/
LuaStackGuard::~LuaStackGuard()
{
    SWEET_ASSERT( lua_state_ );
    lua_settop( lua_state_, restore_to_position_ );
}

/**
// Reset the stack position that this LuaStackGuard will set the top of
// the stack to when it is destroyed.
//
// This can be used to make a %LuaStackGuard have no effect if a function
// has completed successfully.  This lets a %LuaStackGuard be used to 
// cleanup only in an error scenario - if the function completes successfully
// it can call LuaStackGuard::reset_to_top_of_stack() just before it returns
// and the stack will remain unchanged when the %LuaStackGuard is destroyed.
//
// @param restore_to_position_delta
//  The number of stack entries less than the current top of the stack to 
//  restore the top of the stack to when this LuaStackGuard is destroyed.
*/
void LuaStackGuard::reset_to_top_of_stack( int restore_to_position_delta )
{
    restore_to_position_ = lua_gettop(lua_state_) - restore_to_position_delta;
}