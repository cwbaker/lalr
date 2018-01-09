//
// Lua.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUA_HPP_INCLUDED
#define SWEET_LUA_LUA_HPP_INCLUDED

#include "declspec.hpp"
#include "AddMemberHelper.hpp"
#include "AddGlobal.ipp"
#include "AddMember.ipp"
#include "AddParameter.ipp"
#include "LuaInvoker.ipp"
#include "LuaReturner.ipp"
#include "lua_types.hpp"
#include "lua_/lua.h"

namespace sweet
{

namespace lua
{

/**
// A %Lua virtual machine.
*/
class SWEET_LUA_DECLSPEC Lua
{
    lua_State* lua_state_; ///< The main lua_State.
    bool stack_trace_enabled_; ///< Whether or not a stack trace is included as part of error messages.
    AddParameterHelper add_parameter_helper_; ///< The AddParameterHelper that stores state for AddParameter objects.
    AddMemberHelper add_member_helper_; ///< The AddMemberHelper that stores state for AddMember objects.

    public:
        Lua();
        ~Lua();

        lua_State* get_lua_state() const;

        void set_stack_trace_enabled( bool stack_trace_enabled );
        bool is_stack_trace_enabled() const;

        bool is_value( const char* global ) const;
        bool is_boolean( const char* global ) const;
        bool is_number( const char* global ) const;
        bool is_string( const char* global ) const;
        bool is_function( const char* global ) const;
        bool boolean( const char* global ) const;
        int integer( const char* global ) const;
        float number( const char* global ) const;
        std::string string( const char* global ) const;
        AddGlobal globals();
        AddMember table();

        template <class Type> void create( const Type& object );
        template <class Type> void create_with_existing_table( const Type& object );
        template <class Type> void destroy( const Type& object );
        template <class Type> AddMember members( const Type& object );
        template <class Type> bool is_value( const Type& object, const char* field ) const;
        template <class Type> bool is_boolean( const Type& object, const char* field ) const;
        template <class Type> bool is_number( const Type& object, const char* field ) const;
        template <class Type> bool is_string( const Type& object, const char* field ) const;
        template <class Type> bool is_function( const Type& object, const char* field ) const;
        template <class Type> bool boolean( const Type& object, const char* field ) const;
        template <class Type> int integer( const Type& object, const char* field ) const;
        template <class Type> float number( const Type& object, const char* field ) const;
        template <class Type> std::string string( const Type& object, const char* field ) const;

        AddParameter call( lua_Reader reader, void* context, const char* name );
        AddParameter call( const char* filename, const char* name );
        AddParameter call( const char* first, const char* last, const char* name );
        AddParameter call( const char* function );
        AddParameter call( const LuaValue& value );
        template <class Type> AddParameter call( const char* function, const Type& object );

        AddParameter preprocess( const char* filename, const char* name );
        AddParameter preprocess( const char* first, const char* last, const char* name );

    private:
        static int panic( lua_State* lua_state );
};

}

}

#include "Lua.ipp"
#include "AddParameterHelper.ipp"
#include "lua_functions.ipp"

#endif
