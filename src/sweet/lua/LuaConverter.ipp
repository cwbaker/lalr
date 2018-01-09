//
// LuaConverter.ipp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUACONVERTER_IPP_INCLUDED
#define SWEET_LUA_LUACONVERTER_IPP_INCLUDED

#include "LuaConverter.hpp"
#include "LuaObjectConverter.hpp"
#include "LuaThunker.ipp"

namespace sweet
{

namespace lua
{

template <class Type>
void sweet::lua::LuaConverter<Type>::create( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::create( lua_state, value );
}

template <class Type>
void sweet::lua::LuaConverter<Type>::destroy( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::destroy( lua_state, value );
}

template <class Type>
void LuaConverter<Type>::push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state, value );
}

template <class Type>
Type LuaConverter<Type>::to( lua_State* lua_state, int position )
{
    typedef typename traits::traits<Type>::base_type base_type;
    return LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::to( lua_state, position );
}

template <class Type>
void LuaConverter<sweet::lua::LuaValueWrapper<Type> >::push( lua_State* lua_state, const LuaValueWrapper<Type>& value )
{
    LuaObjectConverter<Type, LuaByValue>::push( lua_state, value.get_value() );
}

template <class R>
void LuaConverter<R (*)()>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R>
typename LuaConverter<R (*)()>::Function 
LuaConverter<R (*)()>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template<class R, class A0>
void LuaConverter<R (*)(A0)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0>
typename LuaConverter<R (*)(A0)>::Function 
LuaConverter<R (*)(A0)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template<class R, class A0, class A1>
void LuaConverter<R (*)(A0, A1)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0, class A1>
typename LuaConverter<R (*)(A0, A1)>::Function 
LuaConverter<R (*)(A0, A1)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class A0, class A1, class A2>
void LuaConverter<R (*)(A0, A1, A2)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0, class A1, class A2>
typename LuaConverter<R (*)(A0, A1, A2)>::Function 
LuaConverter<R (*)(A0, A1, A2)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class A0, class A1, class A2, class A3>
void 
LuaConverter<R (*)(A0, A1, A2, A3)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0, class A1, class A2, class A3>
typename LuaConverter<R (*)(A0, A1, A2, A3)>::Function 
LuaConverter<R (*)(A0, A1, A2, A3)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class A0, class A1, class A2, class A3, class A4>
void LuaConverter<R (*)(A0, A1, A2, A3, A4)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0, class A1, class A2, class A3, class A4>
typename LuaConverter<R (*)(A0, A1, A2, A3, A4)>::Function 
LuaConverter<R (*)(A0, A1, A2, A3, A4)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class A0, class A1, class A2, class A3, class A4, class A5>
void LuaConverter<R (*)(A0, A1, A2, A3, A4, A5)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class A0, class A1, class A2, class A3, class A4, class A5>
typename LuaConverter<R (*)(A0, A1, A2, A3, A4, A5)>::Function 
LuaConverter<R (*)(A0, A1, A2, A3, A4, A5)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class T>
void LuaConverter<R (T::*)()>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T>
typename LuaConverter<R (T::*)()>::Function 
LuaConverter<R (T::*)()>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class T>
void LuaConverter<R (T::*)() const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T>
typename LuaConverter<R (T::*)() const>::Function 
LuaConverter<R (T::*)() const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_isuserdata(lua_state, position) && lua_touserdata(lua_state, position) != 0 );
    return *static_cast<Function*>( lua_touserdata(lua_state, position) );
}

template <class R, class T, class A0>
void LuaConverter<R (T::*)(A0)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0>
typename LuaConverter<R (T::*)(A0)>::Function 
LuaConverter<R (T::*)(A0)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0>
void LuaConverter<R (T::*)(A0) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0>
typename LuaConverter<R (T::*)(A0) const>::Function 
LuaConverter<R (T::*)(A0) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1>
void LuaConverter<R (T::*)(A0, A1)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1>
typename LuaConverter<R (T::*)(A0, A1)>::Function 
LuaConverter<R (T::*)(A0, A1)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1>
void LuaConverter<R (T::*)(A0, A1) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1>
typename LuaConverter<R (T::*)(A0, A1) const>::Function 
LuaConverter<R (T::*)(A0, A1) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2>
void LuaConverter<R (T::*)(A0, A1, A2)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2>
typename LuaConverter<R (T::*)(A0, A1, A2)>::Function 
LuaConverter<R (T::*)(A0, A1, A2)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2>
void LuaConverter<R (T::*)(A0, A1, A2) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2>
typename LuaConverter<R (T::*)(A0, A1, A2) const>::Function 
LuaConverter<R (T::*)(A0, A1, A2) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3>
void LuaConverter<R (T::*)(A0, A1, A2, A3)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3>
typename LuaConverter<R (T::*)(A0, A1, A2, A3)>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3>
void LuaConverter<R (T::*)(A0, A1, A2, A3) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3>
typename LuaConverter<R (T::*)(A0, A1, A2, A3) const>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4>
void LuaConverter<R (T::*)(A0, A1, A2, A3, A4)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4>
typename LuaConverter<R (T::*)(A0, A1, A2, A3, A4)>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3, A4)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4>
void LuaConverter<R (T::*)(A0, A1, A2, A3, A4) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4>
typename LuaConverter<R (T::*)(A0, A1, A2, A3, A4) const>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3, A4) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
void LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5)>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
typename LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5)>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5)>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
void LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5) const>::push( lua_State* lua_state, Function function )
{
    SWEET_ASSERT( lua_state != 0 );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    lua_pushcclosure( lua_state, &LuaThunker<Function>::function, 1 );
}

template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
typename LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5) const>::Function 
LuaConverter<R (T::*)(A0, A1, A2, A3, A4, A5) const>::to( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    lua_pushvalue( lua_state, lua_upvalueindex(1) );
    SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
    Function function = *reinterpret_cast<Function*>( lua_touserdata(lua_state, -1) );
    lua_pop( lua_state, 1 );
    return function;
}

}

}

#endif
