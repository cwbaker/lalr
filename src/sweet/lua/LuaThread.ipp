//
// LuaThread.ipp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUATHREAD_IPP_INCLUDED
#define SWEET_LUA_LUATHREAD_IPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// Call a member function in this %LuaThread.
//
// @param function
//  The name of the function to call.
//
// @param object
//  The object to call the member function on.
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
template <class Type>
AddParameter LuaThread::call( const char* function, const Type& object )
{
    SWEET_ASSERT( function );
    return add_parameter_helper_.call<Type>( function, object );
}

/**
// Resume a member function in this %LuaThread.
//
// @param function
//  The name of the function to call.
//
// @param object
//  The object to call the member function on.
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
template <class Type>
AddParameter LuaThread::resume( const char* function, const Type& object )
{
    SWEET_ASSERT( function );
    return add_parameter_helper_.resume<Type>( function, object );
}

}

}

#endif
