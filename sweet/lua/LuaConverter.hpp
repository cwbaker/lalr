//
// LuaConverter.hpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUACONVERTER_HPP_INCLUDED
#define SWEET_LUA_LUACONVERTER_HPP_INCLUDED

#include "declspec.hpp"
#include "LuaValueWrapper.hpp"
#include "lua_/lua.h"
#include <sweet/traits/traits.hpp>
#include <string>

namespace sweet
{

namespace lua
{

class LuaValue;

/**
// @internal
//
// A class template that (with specializations) provides conversion of values 
// between C++ and Lua.
//
// When functions are pushed onto the Lua stack the LuaConverter will push a 
// lua_CFunction thunker that decodes the arguments and function from Lua 
// and dispatches the call to the C++ function.  The C++ function or member 
// function pointer is stored as the first up value of the lua_CFunction.  
// The lua_CFunction is generated from the class template function 
// LuaThunker<Function>::function().  See LuaThunker for more details.
*/
template <class Type>
struct LuaConverter
{
    static void create( lua_State* lua_state, typename traits::traits<Type>::parameter_type value );
    static void destroy( lua_State* lua_state, typename traits::traits<Type>::parameter_type value );
    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value );
    static Type to( lua_State* lua_state, int position );
};

template <class Type>
struct LuaConverter<LuaValueWrapper<Type>>
{
    static void push( lua_State* lua_state, const LuaValueWrapper<Type>& value );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<bool>
{
    static void push( lua_State* lua_state, bool value );
    static bool to( lua_State* lua_state, int position );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<int>
{
    static void push( lua_State* lua_state, int value );
    static int to( lua_State* lua_state, int position );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<float>
{
    static void push( lua_State* lua_state, float value );
    static float to( lua_State* lua_state, int position );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<std::string>
{
    static void push( lua_State* lua_state, const std::string& value );
    static std::string to( lua_State* lua_state, int position );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<const std::string&>
{
    static void push( lua_State* lua_state, const std::string& value );
};

template <>
struct SWEET_LUA_DECLSPEC LuaConverter<const LuaValue&>
{
    static void push( lua_State* lua_state, const LuaValue& value );
};

template<class R>
struct LuaConverter<R (*)()>
{
    typedef R (*Function)();
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0>
struct LuaConverter<R (*)(A0)>
{
    typedef R (*Function)(A0);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0, class A1>
struct LuaConverter<R (*)(A0, A1)>
{
    typedef R (*Function)(A0, A1);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0, class A1, class A2>
struct LuaConverter<R (*)(A0, A1, A2)>
{
    typedef R (*Function)(A0, A1, A2);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0, class A1, class A2, class A3>
struct LuaConverter<R (*)(A0, A1, A2, A3)>
{
    typedef R (*Function)(A0, A1, A2, A3);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0, class A1, class A2, class A3, class A4>
struct LuaConverter<R (*)(A0, A1, A2, A3, A4)>
{
    typedef R (*Function)(A0, A1, A2, A3, A4);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaConverter<R (*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R (*Function)(A0, A1, A2, A3, A4, A5);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T>
struct LuaConverter<R (T::*)()>
{
    typedef R (T::* Function)();
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T>
struct LuaConverter<R (T::*)() const>
{
    typedef R (T::* Function)() const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0>
struct LuaConverter<R (T::*)(A0)>
{
    typedef R (T::* Function)(A0);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0>
struct LuaConverter<R (T::*)(A0) const>
{
    typedef R (T::* Function)(A0) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1>
struct LuaConverter<R (T::*)(A0, A1)>
{
    typedef R (T::* Function)(A0, A1);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1>
struct LuaConverter<R (T::*)(A0, A1) const>
{
    typedef R (T::* Function)(A0, A1) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2>
struct LuaConverter<R (T::*)(A0, A1, A2)>
{
    typedef R (T::* Function)(A0, A1, A2);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2>
struct LuaConverter<R (T::*)(A0, A1, A2) const>
{
    typedef R (T::* Function)(A0, A1, A2) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3>
struct LuaConverter<R (T::*)(A0, A1, A2, A3)>
{
    typedef R (T::* Function)(A0, A1, A2, A3);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3>
struct LuaConverter<R (T::*)(A0, A1, A2, A3) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3, class A4>
struct LuaConverter<R (T::*)(A0, A1, A2, A3, A4)>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3, class A4>
struct LuaConverter<R (T::*)(A0, A1, A2, A3, A4) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4, A5);
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

template<class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4, A5) const;
    static void push( lua_State* lua_state, Function function );
    static Function to( lua_State* lua_state, int position );
};

}

}

#endif
