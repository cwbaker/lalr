//
// persist.ipp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_PERSIST_IPP_INCLUDED
#define SWEET_LUA_PERSIST_IPP_INCLUDED

#include <sweet/persist/lua.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Perform one off initialization needed to %persist types in the %lua 
// component in Archives.
//
// @param archive
//  The Archive to declare types and set contexts with.
//
// @param %lua
//  The Lua object to persist values from.
*/
template <class Archive> 
void enter( Archive& archive, Lua& lua )
{
    archive.set_context( SWEET_STATIC_TYPEID(Lua), &lua );
    lua_State* lua_state = lua.get_lua_state();
    SWEET_ASSERT( lua_state );
    sweet::persist::enter( archive, lua_state );
}

/**
// @internal
//
// Destroy the table used to track tables that have already been loaded.
//
// @param archive
//  The Archive (ignored).
//
// @param lua
//  The Lua object that contains the lua_State to destroy the tracking table
//  in.
*/
template <class Archive> 
void exit( Archive& archive, Lua& lua )
{
    lua_State* lua_state = lua.get_lua_state();
    SWEET_ASSERT( lua_state );
    sweet::persist::exit( archive, lua_state );
}

/**
// @internal
//
// Persist a Lua object.
//
// @param archive
//  The Archive to persist the object with.
//
// @param object
//  The object to persist.
*/
template <class Type, class Storage, class Archive> 
void persist( Archive& archive, const char* name, typename traits::traits<Type>::parameter_type object )
{
    using namespace sweet::persist;
    ObjectGuard<Archive> guard( archive, name, NULL, MODE_VALUE, 1 );

    Lua* lua = reinterpret_cast<Lua*>( archive.get_context(SWEET_STATIC_TYPEID(Lua)) );
    SWEET_ASSERT( lua );

    lua_State* lua_state = lua->get_lua_state();
    SWEET_ASSERT( lua_state );

    LuaStackGuard stack_guard( lua_state );

    if ( archive.is_writing() )
    {
        LuaObjectConverter<Type, Storage>::push( lua_state, object );
    }

    sweet::persist::persist_lua_table( archive, "table", lua_state, false );

    if ( archive.is_reading() )
    {
        LuaObjectConverter<Type, Storage>::create_with_existing_table( lua_state, object );
    }
}

/**
// Persist a LuaObject in an Archive.
// 
// @param archive
//  The Archive to persist the LuaObject in.
*/
template <class Archive> 
void LuaObject::persist( Archive& archive )
{
    lua_ = reinterpret_cast<Lua*>( archive.get_context(SWEET_STATIC_TYPEID(Lua)) );
    SWEET_ASSERT( lua_ );

    lua_State* lua_state = lua_->get_lua_state();
    SWEET_ASSERT( lua_state );

    LuaStackGuard stack_guard( lua_state );

    if ( archive.is_writing() )
    {
        lua_push_object( lua_state, this );
    }

    sweet::persist::persist_lua_table( archive, "table", lua_state, false );

    if ( archive.is_reading() )
    {
        lua_create_object_with_existing_table( lua_state, this );
    }
}

}

}

#endif
