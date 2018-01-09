//
// LuaObject.cpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#include "Lua.hpp"
#include "LuaObject.hpp"

using namespace sweet::lua;

/**
// Constructor.
*/
LuaObject::LuaObject()
: lua_( NULL )
{
}

/**
// Constructor.
//
// @param lua
//  The Lua object that this LuaObject is part of.
*/
LuaObject::LuaObject( Lua& lua )
: lua_( &lua )
{
    SWEET_ASSERT( lua_ );
    lua_->create<LuaObject>( *this );
}

/**
// Destructor.
//
// If this %LuaObject has a non null lua_State then the LuaObject's table is 
// removed from the %Lua registry (the object may still exist but will no 
// longer be able to be reached from the C++ engine).
*/
LuaObject::~LuaObject()
{
    if ( lua_ )
    {
        lua_->destroy<LuaObject>( *this );
    }
}

/**
// Set the members of this LuaObject.
//
// @return
//  An AddMember helper object that provides a convenient syntax for setting
//  member values of this LuaObject.
*/
AddMember LuaObject::members()
{
    SWEET_ASSERT( lua_ );
    return lua_->members<LuaObject>( *this );
}

/**
// Is there a variable named \e field in this LuaObject?
//
// @return
//  True if there is a value named \e field otherwise false.
*/
bool LuaObject::is_value( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->is_value<LuaObject>( *this, field );    
}

/**
// Is the field named \e field a boolean?
//
// @return
//  True if \e field exists and is a boolean value otherwise false.
*/
bool LuaObject::is_boolean( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->is_boolean<LuaObject>( *this, field );
}

/**
// Is the field named \e field a number?
//
// @return
//  True if \e field exists and is a number value otherwise false.
*/
bool LuaObject::is_number( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->is_number<LuaObject>( *this, field );
}

/**
// Is the field named \e field a string?
//
// @return
//  True if \e field exists and is a string otherwise false.
*/
bool LuaObject::is_string( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->is_string<LuaObject>( *this, field );
}

/**
// Is the field named \e field a function?
//
// @return
//  True if \e field exists and is a function otherwise false.
*/
bool LuaObject::is_function( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->is_function<LuaObject>( *this, field );
}

/**
// Get the boolean value of a field.
//
// @param field
//  The name of the field to get (this field must exist).
//
// @return
//  The boolean value of the field.
*/
bool LuaObject::boolean( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->boolean<LuaObject>( *this, field );
}

/**
// Get the integer value of a field.
//
// @param field
//  The name of the field to get (this field must exist and must be able to
//  be converted to a number).
//
// @return
//  The integer value of the field.
*/
int LuaObject::integer( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->integer<LuaObject>( *this, field );
}

/**
// Get the numeric value of a field.
//
// @param field
//  The name of the field to get the value of (this field must exist and must
//  be able to be converted to a number).
//
// @return
//  The numeric value of the field.
*/
float LuaObject::number( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->number<LuaObject>( *this, field );
}

/**
// Get the string value of a field.
//
// @param field
//  The field to get the value of (this field must exist and must be able to
//  be converted to a string).
//
// @return
//  The string value of the field.
*/
std::string LuaObject::string( const char* field ) const
{
    SWEET_ASSERT( lua_ );
    return lua_->string<LuaObject>( *this, field );
}
