//
// LuaInvoker.ipp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAINVOKER_IPP_INCLUDED
#define SWEET_LUA_LUAINVOKER_IPP_INCLUDED

#include "LuaInvoker.hpp"

namespace sweet
{

namespace lua
{

/**
// Call a C++ function with no arguments and push its return value onto the 
// Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
*/
template <class Returner, class R>
int LuaInvoker<Returner, R (*)()>::call( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)() );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with no arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @return
//  Always returns 0.
*/
template <class Returner>
int LuaInvoker<Returner, void (*)()>::call( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)();
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with one argument and push its return value onto the 
// Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class R, class A0>
int LuaInvoker<Returner, R (*)(A0)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)(a0) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class A0>
int LuaInvoker<Returner, void (*)(A0)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)( a0 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments and push its return value onto the 
// Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class R, class A0, class A1>
int LuaInvoker<Returner, R (*)(A0, A1)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    
    try
    {
        return Returner::return_( lua_state, (*function)(a0, a1) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class A0, class A1>
int LuaInvoker<Returner, void (*)(A0, A1)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    
    try
    {
        (*function)( a0, a1 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with three arguments and push its return value onto 
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class R, class A0, class A1, class A2>
int LuaInvoker<Returner, R (*)(A0, A1, A2)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)(a0, a1, a2) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class A0, class A1, class A2>
int LuaInvoker<Returner, void (*)(A0, A1, A2)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)( a0, a1, a2 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with four arguments and push its return value onto 
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
*/
template <class Returner, class R, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, R (*)(A0, A1, A2, A3)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)(a0, a1, a2, a3) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
*/
template <class Returner, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, void (*)(A0, A1, A2, A3)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)( a0, a1, a2, a3 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with five arguments and push its return value onto 
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The firth argument to the call.
*/
template <class Returner, class R, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, R (*)(A0, A1, A2, A3, A4)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)(a0, a1, a2, a3, a4) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
*/
template <class Returner, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, void (*)(A0, A1, A2, A3, A4)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)( a0, a1, a2, a3, a4 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with five arguments and push its return value onto 
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The firth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class R, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, R (*)(A0, A1, A2, A3, A4, A5)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        return Returner::return_( lua_state, (*function)(a0, a1, a2, a3, a4, a5) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ function with two arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, void (*)(A0, A1, A2, A3, A4, A5)>::call( lua_State* lua_state, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (*function)( a0, a1, a2, a3, a4, a5 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with no arguments and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
*/
template <class Returner, class T, class R>
int LuaInvoker<Returner, R (T::*)()>::call( lua_State* lua_state, Function function, T* this_pointer )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)() );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with no arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @return
//  Nothing.
*/
template <class Returner, class T>
int LuaInvoker<Returner, void (T::*)()>::call( lua_State* lua_state, Function function, T* this_pointer )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)();
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ const member function with no arguments and push its return 
// value onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
*/
template <class Returner, class T, class R>
int LuaInvoker<Returner, R (T::*)() const>::call( lua_State* lua_state, Function function, const T* this_pointer )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)() );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ const member function with no arguments.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
*/
template <class Returner, class T>
int LuaInvoker<Returner, void (T::*)() const>::call( lua_State* lua_state, Function function, const T* this_pointer )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)();
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class T, class R, class A0>
int LuaInvoker<Returner, R (T::*)(A0)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class T, class A0>
int LuaInvoker<Returner, void (T::*)(A0)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class T, class R, class A0>
int LuaInvoker<Returner, R (T::*)(A0) const>::call( lua_State* lua_state, Function function, const T* this_pointer, A0 a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ const member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The argument to the call.
*/
template <class Returner, class T, class A0>
int LuaInvoker<Returner, void (T::*)(A0) const>::call( lua_State* lua_state, Function function, const T* this_pointer, A0 a0 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1>
int LuaInvoker<Returner, R (T::*)(A0, A1)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class T, class A0, class A1>
int LuaInvoker<Returner, void (T::*)(A0, A1)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    
    try
    {
        (this_pointer->*function)( a0, a1 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ const member function with one argument and push its return 
// value onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1>
int LuaInvoker<Returner, R (T::*)(A0, A1) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ const member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
*/
template <class Returner, class T, class A0, class A1>
int LuaInvoker<Returner, void (T::*)(A0, A1) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );
    
    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @return
//  Nothing.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3, a4) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3, a4 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );

    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3, a4) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3, class A4>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3, a4 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4, A5)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );
    
    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3, a4, a5) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4, A5)>::call( lua_State* lua_state, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3, a4, a5 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument and push its return value onto
// the Lua stack.
//
// @param lua_state
//  The lua_State to push the return value onto the stack of.
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer to the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4, A5) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );
    SWEET_ASSERT( this_pointer );
    
    try
    {
        return Returner::return_( lua_state, (this_pointer->*function)(a0, a1, a2, a3, a4, a5) );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

/**
// Call a C++ member function with one argument.
//
// @param lua_state
//  The lua_State that has made the call to the C++ function (ignored).
//
// @param function
//  The function to call.
//
// @param this_pointer
//  The this pointer of the object to make the call on.
//
// @param a0
//  The first argument to the call.
//
// @param a1
//  The second argument to the call.
//
// @param a2
//  The third argument to the call.
//
// @param a3
//  The fourth argument to the call.
//
// @param a4
//  The fifth argument to the call.
//
// @param a5
//  The sixth argument to the call.
*/
template <class Returner, class T, class A0, class A1, class A2, class A3, class A4, class A5>
int LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4, A5) const>::call( lua_State* lua_state, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( function );

    try
    {
        (this_pointer->*function)( a0, a1, a2, a3, a4, a5 );
        return Returner::return_( lua_state );
    }

    catch ( const std::exception& exception )
    {
        lua_pushstring( lua_state, exception.what() );
        return lua_error( lua_state );
    }
}

}

}

#endif
