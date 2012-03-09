//
// ptr.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_PTR_HPP_INCLUDED
#define SWEET_LUA_PTR_HPP_INCLUDED

#include <sweet/build.hpp>
#include <sweet/pointer/ptr.hpp>

namespace sweet
{

namespace lua
{

template <class Type>
struct LuaObjectConverter<ptr<Type>, LuaByReference>
{
    static void create( lua_State* lua_state, ptr<Type> value )
    {
        lua_create_object( lua_state, const_cast<Type*>(value.get()) );
    }

    static void create_with_existing_table( lua_State* lua_state, ptr<Type> value )
    {
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(value.get()) );
    }
    
    static void destroy( lua_State* lua_state, ptr<Type> value )
    {
        lua_destroy_object( lua_state, const_cast<Type*>(value.get()) );
    }
    
    static void weaken( lua_State* lua_state, ptr<Type> value )
    {
        lua_weaken_object( lua_state, const_cast<Type*>(value.get()) );
    }

    static void push( lua_State* lua_state, ptr<Type> value )
    {
        lua_push_object( lua_state, const_cast<Type*>(value.get()) );
    }
    
    static ptr<Type> to( lua_State* lua_state, int position )
    {
        SWEET_ASSERT( lua_state );

        ptr<Type> value;
        if ( !lua_isnoneornil(lua_state, position) )
        {
            lua_validate_type( lua_state, position, SWEET_STATIC_TYPEID(Type) );
            lua_getfield( lua_state, position, lua::PTR_KEYWORD );
            SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
            value = lua_to_value<ptr<Type>>( lua_state, -1 );
            lua_pop( lua_state, 1 );
        }
        return value;
    }
};

template <class Type>
void lua_push( lua_State* lua_state, ptr<Type> value )
{    
    typedef traits::traits<Type>::base_type base_type;
    LuaObjectConverter<ptr<Type>, LuaTraits<base_type>::storage_type>::push( lua_state, value );
}

template <class Type>
struct LuaObjectConverter<weak_ptr<Type>, LuaByReference>
{
    static void create( lua_State* lua_state, weak_ptr<Type> weak_value )
    {
        ptr<Type> value( weak_value );
        lua_create_object( lua_state, const_cast<Type*>(value.get()) );
    }

    static void create_with_existing_table( lua_State* lua_state, weak_ptr<Type> weak_value )
    {
        ptr<Type> value( weak_value) ;
        lua_create_object_with_existing_table( lua_state, const_cast<Type*>(value.get()) );
    }
    
    static void destroy( lua_State* lua_state, weak_ptr<Type> weak_value )
    {
        ptr<Type> value( weak_value );
        lua_destroy_object( lua_state, const_cast<Type*>(value.get()) );
    }

    static void weaken( lua_State* lua_state, weak_ptr<Type> weak_value )
    {
        ptr<Type> value( weak_value );
        lua_weaken_object( lua_state, const_cast<Type*>(value.get()) );
    }

    static void push( lua_State* lua_state, weak_ptr<Type> weak_value )
    {
        ptr<Type> value( weak_value );
        lua_push_object( lua_state, const_cast<Type*>(value.get()) );
    }
    
    static weak_ptr<Type> to( lua_State* lua_state, int position )
    {
        SWEET_ASSERT( lua_state );

        weak_ptr<Type> weak_value;
        if ( !lua_isnoneornil(lua_state, position) )
        {
            LuaStackGuard guard( lua_state );
            lua_validate_type( lua_state, position, SWEET_STATIC_TYPEID(Type) );
            lua_getfield( lua_state, position, lua::PTR_KEYWORD );
            SWEET_ASSERT( lua_isuserdata(lua_state, -1) );
            weak_value = lua_to_value<ptr<Type>>( lua_state, -1 );
        }
        return weak_value;
    }
};

template <class Type>
void lua_push( lua_State* lua_state, weak_ptr<Type> weak_value )
{    
    typedef traits::traits<Type>::base_type base_type;
    LuaObjectConverter<weak_ptr<Type>, LuaTraits<base_type>::storage_type>::push( lua_state, weak_value );
}

}

}

#endif
