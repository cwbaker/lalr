//
// LuaThunker.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUATHUNKER_HPP
#define SWEET_LUA_LUATHUNKER_HPP

#include "LuaReturner.hpp"

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Thunk C++ functions called from Lua.
*/
template <class Function, class Returner = LuaReturner<traits::traits<Function>::return_type>, int P0 = 1, int P1 = 2, int P2 = 3, int P3 = 4, int P4 = 5, int P5 = 6, int P6 = 7>
struct LuaThunker
{
};

/**
// @internal
//
// Thunk functions with no arguments.
*/
template <class R, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk functions with one argument.
*/
template <class R, class A0, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk functions with two arguments.
*/
template <class R, class A0, class A1, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0, A1), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};


/**
// @internal
//
// Thunk functions with three arguments.
*/
template <class R, class A0, class A1, class A2, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0, A1, A2), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk functions with four arguments.
*/
template <class R, class A0, class A1, class A2, class A3, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0, A1, A2, A3), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk functions with five arguments.
*/
template <class R, class A0, class A1, class A2, class A3, class A4, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0, A1, A2, A3, A4), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk functions with six arguments.
*/
template <class R, class A0, class A1, class A2, class A3, class A4, class A5, class Returner, int P0, int P1, int P2, int P3, int P4, int P5, int P6>
struct LuaThunker<R (*)(A0, A1, A2, A3, A4, A5), Returner, P0, P1, P2, P3, P4, P5, P6>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with no arguments.
*/
template <class R, class T, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with no arguments.
*/
template <class R, class T, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)() const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with one argument.
*/
template <class R, class T, class A0, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with one argument.
*/
template <class R, class T, class A0, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with two arguments.
*/
template <class R, class T, class A0, class A1, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with two arguments.
*/
template <class R, class T, class A0, class A1, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with three arguments.
*/
template <class R, class T, class A0, class A1, class A2, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with three arguments.
*/
template <class R, class T, class A0, class A1, class A2, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with four arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with four arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with five arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3, A4), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with five arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3, A4) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk member functions with six arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3, A4, A5), Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

/**
// @internal
//
// Thunk const member functions with six arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5, class Returner, int PT, int P0, int P1, int P2, int P3, int P4, int P5>
struct LuaThunker<R (T::*)(A0, A1, A2, A3, A4, A5) const, Returner, PT, P0, P1, P2, P3, P4, P5>
{
    static int function( lua_State* lua_state );
};

}

}

#endif
