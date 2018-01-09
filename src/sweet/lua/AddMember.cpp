//
// AddMember.cpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#include "AddMember.hpp"
#include "AddMemberHelper.hpp"
#include "LuaStackGuard.hpp"
#include "lua_types.hpp"
#include "lua_functions.ipp"
#include <sweet/assert/assert.hpp>

using namespace sweet;
using namespace sweet::lua;

/**
// Constructor.
// 
// @param add_member_helper
//  The AddMemberHelper to reference and use to get a lua_State to add members
//  in (assumed not null).
*/
AddMember::AddMember( AddMemberHelper* add_member_helper )
: add_member_helper_( add_member_helper )
{
    SWEET_ASSERT( add_member_helper_ );
    add_member_helper_->reference();
}

/**
// Destructor.
*/
AddMember::~AddMember()
{
    SWEET_ASSERT( add_member_helper_ );
    add_member_helper_->release();
}

/**
// Set the this pointer member variable.
//
// The this pointer is stored in the "__this" field of the table being 
// modified.  When the C++ object is destroyed the value of the "__this" 
// field is set to nil so that %Lua can no longer reach the C++ object.
//
// Provided that the lifetime of the C++ object and table are tied to each
// other (e.g. the Lua::create(), Lua::create_with_existing_table(), and
// Lua::destroy() are called to notify the library of the creation and 
// destruction of a C++ object) applications that are using the "__this" 
// field to get back to an actual C++ object can determine whether or not 
// that C++ object is still valid by checking for the "__this" field in the 
// table - if it is nil then the C++ object has been destroyed.
//
// This is important because the table used to represent the C++ object in the 
// Lua virtual machine may outlive the C++ object.  A typical example of this
// would be C++ objects that were set as the value of global variables and 
// used to provide a scripting API for an application.  When the C++ object
// is destroyed it removes the reference to the table from the %Lua registry 
// but doesn't know anything about the global variable that is also
// referencing the table - so the table will carry on being valid even 
// though its associated C++ object has been destroyed.
//
// @param value
//  The value of the this pointer to add.
//
// @return
//  This AddMember object.
*/
sweet::lua::AddMember& AddMember::this_pointer( void* value )
{
    SWEET_ASSERT( add_member_helper_ );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushlightuserdata( lua_state, value );
    lua_setfield( lua_state, -2, THIS_KEYWORD );
        
    return *this;
}

/**
// Set the type member variable.
//
// The type is used to validate that the C++ object assocaited with the
// table is of the correct type when it is passed back to C++ 
// (see lua_validate_type()).
//
@code
Foo foo;
Lua lua;
lua.create( foo );
lua.members( foo )       
    .type( SWEET_STATIC_TYPEID(Foo) )
    .this_pointer( &foo )
;
@endcode
//
// @param type
//  The rtti::Type to set the LuaObject being created to be of.
//
// @return
//  This AddMember.
*/
AddMember& AddMember::type( const rtti::Type& type )
{
    SWEET_ASSERT( add_member_helper_ != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_push_value<rtti::Type>( lua_state, type );
    lua_setfield( lua_state, -2, TYPE_KEYWORD );    
    
    return *this;
}

/**
// Set a member variable to nil.
//
// @param name
//  The name of the variable to set.
//
// @param nil
//  The LuaNil used to overload this function (ignored).
//
// @return
//  This AddMember.
*/
AddMember& AddMember::operator()( const char* name, const LuaNil& nil )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushnil( lua_state );
    lua_setfield( lua_state, -2, name );

    return *this;
}

/**
// Set a member variable to the global environment.
//
// @param name
//  The name of the variable to set.
//
// @param global_environment
//  The LuaGlobalEnvironment used to overload this function (ignored).
//
// @return
//  This AddMember.
*/
AddMember& AddMember::operator()( const char* name, const LuaGlobalEnvironment& global_environment )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushvalue( lua_state, LUA_GLOBALSINDEX );
    lua_setfield( lua_state, -2, name );

    return *this;
}

/**
// Set a member variable to a boolean value.
//
// @param name
//  The name of the variable to add.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, bool value )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushboolean( lua_state, value ? 1 : 0 );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}

/**
// Set a member variable to an integer value.
//
// @param name
//  The name of the variable to add.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, int value )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushnumber( lua_state, static_cast<lua_Number>(value) );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}

/**
// Set a member variable to a floating point value.
//
// @param name
//  The name of the variable to add.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, float value )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushnumber( lua_state, static_cast<lua_Number>(value) );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}

/**
// Set a member variable to a string value.
//
// @param name
//  The name of the variable to add.
//
// @param value
//  The string to add (assumed to be not null and null terminated).
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, const char* value )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );
    SWEET_ASSERT( value );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushstring( lua_state, value );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}

/**
// Set a member variable to a string value.
//
// @param name
//  The name of the variable to add.
//
// @param value
//  The value of the variable to add.
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, const std::string& value )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushlstring( lua_state, value.c_str(), value.length() );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}

/**
// Set a member variable to refer to a raw function with no up values.
//
// @param name
//  The name to store the variable under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @return
//  This AddMember object.
*/
AddMember& AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper )
{
    SWEET_ASSERT( add_member_helper_ );
    SWEET_ASSERT( name );
    
    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 0 );
    lua_setfield( lua_state, -2, name );
    
    return *this;
}
