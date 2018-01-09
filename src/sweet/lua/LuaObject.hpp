//
// LuaObject.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
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

#endif
