//
// LuaObjectConverter.hpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAOBJECTCONVERTER_HPP_INCLUDED
#define SWEET_LUA_LUAOBJECTCONVERTER_HPP_INCLUDED

#include "LuaTraits.hpp"
#include "lua_functions.hpp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A class template that (with specializations) provides creation, 
// destruction, and conversion of object values between C++ and Lua.
*/
template <class Type, class Storage>
struct LuaObjectConverter
{
};

template <class Type>
struct LuaObjectConverter<Type, LuaByValue>
{
    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_push_value<Type>( lua_state, value );
    }
    
    static Type& to( lua_State* lua_state, int position )
    {
        return lua_to_value<Type>( lua_state, position );
    }
};

template <class Type>
struct LuaObjectConverter<Type, LuaByReference>
{
    static void create( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static void create_with_existing_table( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(&value) );
    }
    
    static void destroy( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(&value) );
    }

    static void weaken( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(&value) );
    }

    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_push_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static Type& to( lua_State* lua_state, int position )
    {
        void* object = lua_to_object( lua_state, position, SWEET_STATIC_TYPEID(Type) );
        SWEET_ASSERT( object );
        return *reinterpret_cast<Type*>( object );
    }
};

template <class Type>
struct LuaObjectConverter<Type&, LuaByValue>
{
    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_push_value( lua_state, value );
    }
    
    static Type& to( lua_State* lua_state, int position )
    {
        return lua_to_value<Type>( lua_state, position );
    }
};

template <class Type>
struct LuaObjectConverter<Type&, LuaByReference>
{
    static void create( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static void create_with_existing_table( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(&value) );
    }
    
    static void destroy( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(&value) );
    }

    static void weaken( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(&value) );
    }

    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_push_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static Type& to( lua_State* lua_state, int position )
    {
        void* object = lua_to_object( lua_state, position, SWEET_STATIC_TYPEID(Type) );
        SWEET_ASSERT( object );
        return *reinterpret_cast<Type*>( object );
    }
};

template <class Type>
struct LuaObjectConverter<const Type&, LuaByValue>
{
    static void push( lua_State* lua_state, const Type& value )
    {
        lua_push_value<Type>( lua_state, value );
    }
    
    static const Type& to( lua_State* lua_state, int position )
    {
        return lua_to_value<Type>( lua_state, position );
    }
};

template <class Type>
struct LuaObjectConverter<const Type&, LuaByReference>
{
    static void create( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static void create_with_existing_table( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(&value) );
    }
    
    static void destroy( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(&value) );
    }

    static void weaken( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(&value) );
    }

    static void push( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
    {
        lua_push_object( lua_state, const_cast<Type*>(&value) );
    }
    
    static const Type& to( lua_State* lua_state, int position )
    {
        void* object = lua_to_object( lua_state, position, SWEET_STATIC_TYPEID(Type) );
        SWEET_ASSERT( object != NULL );
        return *reinterpret_cast<Type*>( object );
    }
};

template <class Type>
struct LuaObjectConverter<Type*, LuaByValue>
{
    static void push( lua_State* lua_state, const Type* value )
    {
        lua_pushlightuserdata( lua_state, const_cast<Type*>(value) );
    }
    
    static Type* to( lua_State* lua_state, int position )
    {
        SWEET_ASSERT( lua_isuserdata(lua_state, position) );
        return reinterpret_cast<Type*>( lua_touserdata(lua_state, position) );
    }
};

template <class Type>
struct LuaObjectConverter<Type*, LuaByReference>
{
    static void create( lua_State* lua_state, const Type* value )
    {
        lua_create_object( lua_state, const_cast<Type*>(value) );
    }
    
    static void create_with_existing_table( lua_State* lua_state, const Type* value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(value) );
    }
    
    static void destroy( lua_State* lua_state, const Type* value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(value) );
    }

    static void weaken( lua_State* lua_state, const Type* value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(value) );
    }

    static void push( lua_State* lua_state, const Type* value )
    {
        lua_push_object( lua_state, const_cast<Type*>(value) );
    }
    
    static Type* to( lua_State* lua_state, int position )
    {
        void* object = lua_to_object( lua_state, position, SWEET_STATIC_TYPEID(Type) );
        return reinterpret_cast<Type*>( object );
    }
};

template <class Type>
struct LuaObjectConverter<const Type*, LuaByValue>
{
    static void push( lua_State* lua_state, const Type* value )
    {
        lua_pushlightuserdata( lua_state, const_cast<Type*>(value) );
    }
    
    static const Type* to( lua_State* lua_state, int position )
    {
        SWEET_ASSERT( lua_isuserdata(lua_state, position) );
        return reinterpret_cast<const Type*>( lua_touserdata(lua_state, position) );
    }
};

template <class Type>
struct LuaObjectConverter<const Type*, LuaByReference>
{
    static void create( lua_State* lua_state, const Type* value )
    {
        lua_create_object( lua_state, const_cast<Type*>(value) );
    }
    
    static void create_with_existing_table( lua_State* lua_state, const Type* value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(value) );
    }
    
    static void destroy( lua_State* lua_state, const Type* value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(value) );
    }

    static void weaken( lua_State* lua_state, const Type* value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(value) );
    }

    static void push( lua_State* lua_state, const Type* value )
    {
        lua_push_object( lua_state, const_cast<Type*>(value) );
    }
    
    static const Type* to( lua_State* lua_state, int position )
    {
        void* object = lua_to_object( lua_state, position, SWEET_STATIC_TYPEID(Type) );
        return reinterpret_cast<const Type*>( object );
    }
};

}

}

#endif