//
// LuaObject.hpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAOBJECT_HPP_INCLUDED
#define SWEET_LUA_LUAOBJECT_HPP_INCLUDED

#include "declspec.hpp"
#include "AddMember.hpp"
#include <string>

namespace sweet
{

namespace lua
{

class Lua;

/**
// A %Lua object table.
*/
class SWEET_LUA_DECLSPEC LuaObject
{
    Lua* lua_; ///< The %Lua object that this %LuaObject is part of.
    
    public:
        LuaObject();
        LuaObject( Lua& lua );
        ~LuaObject();

        AddMember members();
        bool is_value( const char* field ) const;
        bool is_boolean( const char* field ) const;
        bool is_number( const char* field ) const;
        bool is_string( const char* field ) const;
        bool is_function( const char* field ) const;
        bool boolean( const char* field ) const;
        int integer( const char* field ) const;
        float number( const char* field ) const;
        std::string string( const char* field ) const;

        template <class Archive> void persist( Archive& archive );

    private:
        LuaObject( const LuaObject& lua_object );
        LuaObject& operator=( const LuaObject& lua_object );
};

}

}

/**
// Persist this LuaObject in an Archive.
// 
// @param archive
//  The Archive to persist this LuaObject in.
*/
template <class Archive> 
void sweet::lua::LuaObject::persist( Archive& archive )
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

#endif
