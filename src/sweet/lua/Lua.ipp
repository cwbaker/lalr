//
// Lua.ipp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUA_IPP_INCLUDED
#define SWEET_LUA_LUA_IPP_INCLUDED

#include "LuaObjectConverter.hpp"
#include "LuaStackGuard.hpp"

namespace sweet
{

namespace lua
{

/**
// Create a table that corresponds to \e object.
//
// Creates a table and sets it as a value in the %Lua registry stored under 
// the address of \e object.
//
// @param object
//  The object to associate the table with.
*/
template <class Type> 
void Lua::create( const Type& object )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::create( lua_state_, object );
}

/**
// Associate the table at the top of the %Lua stack with \e object.
//
// Sets the table at the top of the stack as a value in the %Lua registry 
// stored under the address of \e object.
//
// @param object
//  The object to associate the table with.
*/
template <class Type> 
void Lua::create_with_existing_table( const Type& object )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::create_with_existing_table( lua_state_, object );
}

/**
// Destroy the table that corresponds to \e object.
//
// @param object
//  The object to destroy that associated table of.
*/
template <class Type> 
void Lua::destroy( const Type& object )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::destroy( lua_state_, object );
}

/**
// Set members of the table associated with \e object.
//
// @return
//  An %AddMember helper object that provides a convenient syntax for setting
//  values on the table.
*/
template <class Type>
AddMember Lua::members( const Type& object )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    return AddMember( &add_member_helper_ );
}

/**
// Is there a variable named \e field in the table associated with \e object?
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to check for existence of.
//
// @return
//  True if there is a value named \e field in the table associated with
//  \e object otherwise false.
*/
template <class Type>
bool Lua::is_value( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return !lua_isnil( lua_state_, -1 ) ? true : false;
}

/**
// Is the field named \e field a boolean in the table associated with \e object?
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to check for being a boolean.
//
// @return
//  True if \e field exists and is a boolean otherwise false.
*/
template <class Type>
bool Lua::is_boolean( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_isboolean( lua_state_, -1 ) ? true : false;
}

/**
// Is the field named \e field a number in the table associated with 
// \e object?
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to check for existence of.
//
// @return
//  True if \e field exists and is a number otherwise false.
*/
template <class Type>
bool Lua::is_number( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_isnumber( lua_state_, -1 ) ? true : false;
}


/**
// Is the field named \e field a string in the table associated with 
// \e object?
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to check for existence of.
//
// @return
//  True if \e field exists and is a string otherwise false.
*/
template <class Type>
bool Lua::is_string( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_isstring( lua_state_, -1 ) ? true : false;
}

/**
// Is the field named \e field a function in the table associated with 
// \e object?
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to check for existence of.
//
// @return
//  True if \e field exists and is a function otherwise false.
*/
template <class Type>
bool Lua::is_function( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_isfunction( lua_state_, -1 ) ? true : false;
}


/**
// Get the boolean value of a field in the table associated with \e object.
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to get (this field must exist).
//
// @return
//  The boolean value of the field.
*/
template <class Type>
bool Lua::boolean( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_toboolean( lua_state_, -1 ) ? true : false;
}

/**
// Get the integer value of a field in the table associated with \e object.
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to get (this field must exist and must be able to
//  be converted to a number).
//
// @return
//  The integer value of the field.
*/
template <class Type>
int Lua::integer( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_tointeger( lua_state_, -1 );
}

/**
// Get the numeric value of a field in the table associated with \e object.
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The name of the field to get the value of (this field must exist and must
//  be able to be converted to a number).
//
// @return
//  The numeric value of the field.
*/
template <class Type>
float
Lua::number( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return static_cast<float>( lua_tonumber(lua_state_, -1) );
}

/**
// Get the string value of a field in the able associated with \e object.
//
// @param object
//  The object to find the associated table of.
//
// @param field
//  The field to get the value of (this field must exist and must be able to
//  be converted to a string).
//
// @return
//  The string value of the field.
*/
template <class Type>
std::string Lua::string( const Type& object, const char* field ) const
{
    SWEET_ASSERT( field );

    LuaStackGuard guard( lua_state_ );
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type, typename LuaTraits<base_type>::storage_type>::push( lua_state_, object );
    lua_getfield( lua_state_, -1, field );
    return lua_tostring( lua_state_, -1 );
}

/**
// Call a member function in the main thread of this %Lua virtual machine.
//
// @param function
//  The name of the function to call.
//
// @param object
//  The object to call the member function on.
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
template <class Type> 
AddParameter Lua::call( const char* function, const Type& object )
{
    SWEET_ASSERT( function );
    return add_parameter_helper_.call<Type>( function, object );    
}

}

}

#endif
