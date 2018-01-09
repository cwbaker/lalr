//
// lua_functions.cpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#include "lua_types.hpp"
#include "lua_functions.hpp"
#include "LuaUserData.hpp"
#include "LuaConverter.hpp"
#include "LuaStackGuard.hpp"
#include "LuaUserDataTemplate.ipp"
#include <sweet/rtti/Type.hpp>
#include <sweet/assert/assert.hpp>
#include <stdio.h>

namespace sweet
{

namespace lua
{

/**
// Dump the stack for the lua_State \e lua_state.
//
// @param lua_state
//  The lua_State to dump the stack for.
*/
void lua_dump_stack( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    for ( int i = lua_gettop(lua_state); i > 0; --i )
    {
        if ( lua_type(lua_state, i) == LUA_TSTRING )
        {
            printf( "%d, %s, '%s'\n", i, lua_typename(lua_state, lua_type(lua_state, i)), lua_tostring(lua_state, i) );
        }
        else if ( lua_type(lua_state, i) == LUA_TNUMBER )
        {
            printf( "%d, %s, %d %.02f\n", i, lua_typename(lua_state, lua_type(lua_state, i)), static_cast<int>(lua_tointeger(lua_state, i)), static_cast<float>(lua_tonumber(lua_state, i)) );
        }
        else
        {
            printf( "%d, %s, 0x%p\n", i, lua_typename(lua_state, lua_type(lua_state, i)), lua_topointer(lua_state, i) );
        }
    }
}

/**
// Validate that an item on the stack is able to be converted to a 
// specific type.
//
// If the item on the stack is user data or a table that is of the type 
// specified by \e type then this function will return otherwise it will 
// raise an %error by calling lua_error().
//
// The type of a table that corresponds to an object is set using 
// LuaObject::members() and AddMember::type().
//
// @param lua_state
//  The lua_State to check the object on the stack of.
//
// @param position
//  The position of the item on the %Lua stack.
//
// @param type
//  The rtti::Type of the object to check for being a matching type.
*/
void lua_validate_type( lua_State* lua_state, int position, const rtti::Type& type )
{
    SWEET_ASSERT( lua_state );

    if ( lua_isuserdata(lua_state, position) )
    {
        LuaUserData* user_data = static_cast<LuaUserData*>( lua_touserdata(lua_state, position) );
        SWEET_ASSERT( user_data );
        if ( user_data->type() != type )
        {
            lua_pushfstring( lua_state, "The type '%s' at position %d is not '%s' as expected", user_data->type().name(), position, type.name() );
            lua_error( lua_state );
        }
    }
    else if ( lua_istable(lua_state, position) )
    {
        lua_getfield( lua_state, position, TYPE_KEYWORD );
        
        if ( lua_isnoneornil(lua_state, -1) )
        {
            lua_pushfstring( lua_state, "The '%s' field for the table at position %d is not present", TYPE_KEYWORD, position );
            lua_error( lua_state );
        }       
        
        if ( !lua_isuserdata(lua_state, -1) )
        {
            lua_pushfstring( lua_state, "The '%s' field for the table at position %d is not user data as expected", TYPE_KEYWORD, position );
            lua_error( lua_state );
        }
        
        LuaUserData* user_data = static_cast<LuaUserData*>( lua_touserdata(lua_state, -1) );
        SWEET_ASSERT( user_data != NULL );
        if ( user_data->type() != SWEET_STATIC_TYPEID(rtti::Type) )
        {
            lua_pushfstring( lua_state, "The '%s' field for the table at position %d is not 'rtti::Type' as expected", TYPE_KEYWORD, position );
            lua_error( lua_state );
        }

        LuaUserDataTemplate<rtti::Type>* type_user_data = static_cast<LuaUserDataTemplate<rtti::Type>*>( lua_touserdata(lua_state, -1) );
        SWEET_ASSERT( type_user_data != NULL );
        if ( type_user_data->value() != type )
        {
            lua_pushfstring( lua_state, "The type '%s' at position %d is not '%s' as expected", type_user_data->value().name(), position, type.name() );
            lua_error( lua_state );
        }
               
        lua_pop( lua_state, 1 );
    }
    else
    {
        lua_pushfstring( lua_state, "The type '%s' at position %d is not '%s' as expected", lua_typename(lua_state, lua_type(lua_state, position)), position, type.name() );
        lua_error( lua_state );
    }
}

/**
// @internal
//
// Push a bool onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the bool onto the stack of.
//
// @param value
//  The bool to push.
*/
void lua_push( lua_State* lua_state, bool value )
{
    SWEET_ASSERT( lua_state );
    lua_pushboolean( lua_state, value ? 1 : 0 );
}

/**
// @internal
//
// Push an int onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the int onto the stack of.
//
// @param value
//  The int to push.
*/
void lua_push( lua_State* lua_state, int value )
{
    SWEET_ASSERT( lua_state );
    lua_pushinteger( lua_state, static_cast<lua_Integer>(value) );
}

/**
// @internal
//
// Push a float onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the float onto the stack of.
//
// @param value
//  The float to push.
*/
void lua_push( lua_State* lua_state, float value )
{
    SWEET_ASSERT( lua_state );
    lua_pushnumber( lua_state, static_cast<lua_Number>(value) );
}

/**
// @internal
//
// Push a string onto the Lua stack.
//
// @param lua_state
//  The lua_State to push the string onto the stack of.
//
// @param value
//  The string to push.
*/
void lua_push( lua_State* lua_state, const std::string& value )
{
    SWEET_ASSERT( lua_state );
    lua_pushlstring( lua_state, value.c_str(), value.length() );
}

void lua_push( lua_State* lua_state, const LuaValue& value )
{
    SWEET_ASSERT( lua_state );
    LuaConverter<const LuaValue&>::push( lua_state, value );
}

/**
// @internal
//
// Create a Lua object in \e lua_state identified by \e object.
//
// @param lua_state
//  The lua_State to create the object in.
//
// @param object
//  The address to use to identify the object.
*/
void lua_create_object( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );
    LuaStackGuard guard( lua_state );    
    lua_newtable( lua_state );
    lua_create_object_with_existing_table( lua_state, object );
}

/**
// @internal
//
// Create a Lua object in \e lua_state identified by \e object using the table
// value currently at the top of the Lua stack in \e lua_state.
//
// Assumes that the value at the top of the Lua stack is a table.
//
// The table at the top of the stack is left popped from the stack when this
// function returns.
//
// @param lua_state
//  The lua_State to create the object in.
//
// @param object
//  The address to use to identify the object.
*/
void lua_create_object_with_existing_table( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( lua_istable(lua_state, -1) );
    lua_pushlightuserdata( lua_state, object );
    lua_insert( lua_state, -2 );
    lua_settable( lua_state, LUA_REGISTRYINDEX );
}

/**
// @internal
//
// Destroy the Lua object in \e lua_state identified by \e object.
//
// Removes references to the Lua table for the object from the Lua registry
// and the weak references table.
//
// @param lua_state
//  The lua_State to destroy the object in.
//
// @param object
//  The address to use to identify the object.
*/
void lua_destroy_object( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );
    LuaStackGuard guard( lua_state );

//
// Set the value of the field stored under the THIS_POINTER key to nil.
// See AddMember::this_pointer().  This stops the object being able to be used
// to refer back to an object in C++.
//
    lua_pushlightuserdata( lua_state, object );
    lua_gettable( lua_state, LUA_REGISTRYINDEX );
    if ( !lua_istable(lua_state, -1) )
    {
        lua_pop( lua_state, 1 );
        lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
        lua_pushlightuserdata( lua_state, object );
        lua_gettable( lua_state, -2 );
    }
    if ( lua_istable(lua_state, -1) )
    {
        lua_pushstring( lua_state, THIS_KEYWORD );
        lua_pushnil( lua_state );
        lua_rawset( lua_state, -3 );
    }   
    lua_pop( lua_state, 1 );

    lua_remove_object( lua_state, object );
}

/**
// Remove references to Lua tables from \e object.
//
// @param lua_state
//  The lua_State to remove references to Lua tables from \e object in 
//  (assumed not null).
//
// @param object
//  The address to remove the associated Lua tables of.
*/
void lua_remove_object( lua_State* lua_state, void* object )
{
    // Remove any reference to the object object from the Lua registry.
    lua_pushlightuserdata( lua_state, object );
    lua_pushnil( lua_state );
    lua_settable( lua_state, LUA_REGISTRYINDEX );

    // Remove any reference to the object from the weak objects table.
    lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
    if ( lua_istable(lua_state, -1) )
    {
        lua_pushlightuserdata( lua_state, object );
        lua_pushnil( lua_state );
        lua_settable( lua_state, -3 );    
    }
    lua_pop( lua_state, 1 );
}

/**
// Swap the Lua objects associated with \e object and \e other_object.
//
// Swaps the values referenced by the two addresses in the Lua registry so 
// that the Lua table that is associated with \e object is swapped with the
// Lua table that is associated with \e other_object and vice versa.
//
// The strong/weak relationship from the C++ address to the Lua table is 
// *not* swapped.  For example if \e object has a strong relationship from 
// C++ to the first Lua table and \e other_object has a weak relationship 
// from C++ to the second Lua table then, after swapping, \e object has a 
// strong relationship from C++ to the second Lua table and \e other_object 
// has a weak relationship from C++ to the first Lua table.  The values are 
// swapped but the strength/weakness of the relationship from \e object and
// \e other_object to their Lua tables remains unchanged.
//
// @param object
//  The address used to associate with the first Lua object.
//
// @param other_object
//  The address used to associate with the second Lua object.
*/
void lua_swap_object( lua_State* lua_state, void* object, void* other_object )
{
    LuaStackGuard guard( lua_state );

    // Record the strength of the relationships from the first and second 
    // objects in C++ to their Lua tables are weak so that it can be
    // restored after swapping.
    bool object_weak = lua_is_weak_object( lua_state, object );
    bool other_object_weak = lua_is_weak_object( lua_state, other_object );

    // Push the Lua table associated with the first object onto the stack and 
    // remove it from the Lua registry or weak objects table.
    lua_pushlightuserdata( lua_state, other_object );
    lua_push_object( lua_state, object );
    lua_remove_object( lua_state, object );

    // Push the Lua table associated with the second object onto the stack and
    // remove it from the Lua registry or weak objects table.
    lua_pushlightuserdata( lua_state, object );
    lua_push_object( lua_state, other_object );
    lua_remove_object( lua_state, other_object );

    // Associate the Lua table previously associated with the second object 
    // with the first object.
    lua_settable( lua_state, LUA_REGISTRYINDEX );
    
    // Associate the Lua table previously associated with the first object 
    // with the second object.
    lua_settable( lua_state, LUA_REGISTRYINDEX );

    // Restore weak relationships from the first and second objects to their
    // associated Lua tables.  The strength of the relationship remains as it
    // was before this function was called; i.e. the values are swapped but 
    // the strengths of the relationships from C++ to those values are not.   
    if ( object_weak )
    {
        lua_weaken_object( lua_state, object );
    }
    
    if ( other_object_weak )
    {
        lua_weaken_object( lua_state, other_object );
    }
}

/**
// Is there an object created for \e object in \e lua_state?
//
// @return
//  True if there is a table in the Lua registry under the lightuserdata key
//  \e object otherwise false.
*/
bool lua_is_object( lua_State* lua_state, void* object )
{
    LuaStackGuard guard( lua_state );
    lua_pushlightuserdata( lua_state, object );
    lua_gettable( lua_state, LUA_REGISTRYINDEX );
    if ( lua_isnil(lua_state, -1) )
    {
        lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
        lua_pushlightuserdata( lua_state, object );
        lua_gettable( lua_state, -2 );
    }    
    return lua_istable( lua_state, -1 );
}

/**
// Is there a weak object created for \e object in \e lua_state?
//
// @return
//  True if there is a table in the Lua weak objects table under the 
//  lightuserdata key \e object otherwise false.
*/
bool lua_is_weak_object( lua_State* lua_state, void* object )
{
    LuaStackGuard guard( lua_state );
    lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
    lua_pushlightuserdata( lua_state, object );
    lua_gettable( lua_state, -2 );
    return lua_istable( lua_state, -1 );
}

/**
// Is the value at \e position a table related to an object of type \e type?
//
// @param lua_state
//  The lua_State to check the type of the table/object in.
//
// @param position
//  The stack position to check the type of the table/object at.
//
// @param type
//  The type to check for.
//
// @return
//  True if the value at \e position is a table for a C++ object of type 
//  \e type otherwise false.
*/
bool lua_is_object( lua_State* lua_state, int position, const rtti::Type& type )
{
    SWEET_ASSERT( lua_state );
    
    if ( lua_istable(lua_state, position) )
    {
        LuaStackGuard guard( lua_state );
        lua_getfield( lua_state, position, TYPE_KEYWORD );        
        if ( !lua_isnoneornil(lua_state, -1) && lua_isuserdata(lua_state, -1) )
        {
            LuaUserDataTemplate<rtti::Type>* type_user_data = static_cast<LuaUserDataTemplate<rtti::Type>*>( lua_touserdata(lua_state, -1) );
            SWEET_ASSERT( type_user_data );
            return type_user_data->type() == SWEET_STATIC_TYPEID(rtti::Type) && type_user_data->value() == type;
        }               
    }
    return false;
}

/**
// Weaken the object in \e lua_state identified by \e object.
//
// This moves the table associated with \e object from the Lua registry into
// the weak objects table.  The weak objects table stores only weak references
// to its contents.  This means that the table associated with \e object will
// be able to be garbage collected once there are no more references to it
// from Lua.
//
// @param lua_state
//  The lua_State to weaken the object in.
//
// @param object
//  The address to use to identify the object.
*/
void lua_weaken_object( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );

    if ( object )
    {
        LuaStackGuard guard( lua_state );
        
    //
    // Get the weak objects table from the Lua registry.
    //
        lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
        SWEET_ASSERT( lua_istable(lua_state, -1) );

    //
    // If there is a table for the object in the Lua registry then move that
    // table from the registry to the weak objects table otherwise assume that
    // the object is already weakened and its table already exists in the
    // weak objects table and quietly do nothing.
    //
        lua_pushlightuserdata( lua_state, object );
        lua_gettable( lua_state, LUA_REGISTRYINDEX );
        if ( lua_istable(lua_state, -1) )
        {
            // Add the object's table to the weak objects table.
            lua_pushlightuserdata( lua_state, object );
            lua_pushvalue( lua_state, -2 );
            lua_settable( lua_state, -4 );

            // Remove the object's table from the Lua registry.
            lua_pushlightuserdata( lua_state, object );
            lua_pushnil( lua_state );
            lua_settable( lua_state, LUA_REGISTRYINDEX );
        }
    }
}

/**
// Strengthen the object in \e lua_state identified by \e object.
//
// This moves the table associated with \e object from the weak objects
// table back to the Lua registry.
//
// @param lua_state
//  The lua_State to weaken the object in.
//
// @param object
//  The address to use to identify the object.
*/
void lua_strengthen_object( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );

    if ( object )
    {
        LuaStackGuard guard( lua_state );
        
        lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
        const int WEAK_OBJECTS = lua_gettop( lua_state );
        SWEET_ASSERT( lua_istable(lua_state, WEAK_OBJECTS) );

        lua_pushlightuserdata( lua_state, object );
        lua_rawget( lua_state, WEAK_OBJECTS );
        const int OBJECT = lua_gettop( lua_state );
        if ( lua_istable(lua_state, OBJECT) )
        {
            lua_pushlightuserdata( lua_state, object );
            lua_pushvalue( lua_state, OBJECT );
            lua_rawset( lua_state, LUA_REGISTRYINDEX );

            lua_pushlightuserdata( lua_state, object );
            lua_pushnil( lua_state );
            lua_rawset( lua_state, WEAK_OBJECTS );
        }
    }
}

/**
// @internal
//
// Push \e object's equivalent table onto the stack in \e lua_state.
//
// @param lua_state
//  The lua_State to push the object onto the stack of.
//
// @param object
//  The address to use to identify the object.
//
// @return
//  True if there was a table corresponding to \e object in \e lua_state otherwise
//  false.
*/
bool lua_push_object( lua_State* lua_state, void* object )
{
    SWEET_ASSERT( lua_state );
    
    if ( object )
    {
        lua_pushlightuserdata( lua_state, object );
        lua_gettable( lua_state, LUA_REGISTRYINDEX );
        SWEET_ASSERT( lua_istable(lua_state, -1) || lua_isnil(lua_state, -1) );
        
        if ( lua_isnil(lua_state, -1) )
        {
            lua_pop( lua_state, 1 );
            lua_getfield( lua_state, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
            SWEET_ASSERT( lua_istable(lua_state, -1) );
            lua_pushlightuserdata( lua_state, object );
            lua_gettable( lua_state, -2 );
            lua_remove( lua_state, -2 );
            if ( !lua_istable(lua_state, -1) )
            {
                // There is no entry in the Lua registry for the object at 
                // this address so just push nil and let the type checking
                // handle reporting an error.  This is usually due to no Lua 
                // table having been created for the C++ object.
                lua_pop( lua_state, 1 );
                lua_pushnil( lua_state );
            }         
        }
    }
    else
    {
        lua_pushnil( lua_state );
    }
    return lua_istable( lua_state, -1 );
}

/**
// @internal
//
// Get the address of the object at \e position in \e lua_state's stack 
// without doing any type checking.
//
// @param lua_state
//  The lua_State to push the object onto the stack of.
//
// @param position
//  The absolute position in the stack to get the object from (it is assumed
//  that this position is an absolute position; that is \e position > 0 or
//  position < LUA_REGISTRYINDEX (-10000)).
//
// @return
//  The address of the object or null if there is no value at that position
//  on the stack or if the value at that position is nil.
*/
void* lua_to_object( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( position > 0 || position < LUA_REGISTRYINDEX );

    void* object = NULL;
    if ( !lua_isnoneornil(lua_state, position) )
    {
        lua_pushstring( lua_state, lua::THIS_KEYWORD );
        lua_rawget( lua_state, position );
        if ( lua_islightuserdata(lua_state, -1) )
        {
            object = lua_touserdata( lua_state, -1 );
            lua_pop( lua_state, 1 );
        }
        else if ( lua_isnil(lua_state, -1) )
        {
            lua_pushfstring( lua_state, "Missing this pointer for Lua object at position %d (not set or C++ object has been destroyed)", position );
            lua_error( lua_state );
        }
        else
        {
            lua_pushfstring( lua_state, "Invalid this pointer for Lua object at position %d (type=%s)", position, lua_typename(lua_state, lua_type(lua_state, position)) );
            lua_error( lua_state );
        }
    }
    return object;
}

/**
// @internal
//
// Get the address of the object at \e position in \e lua_state's stack.
//
// @param lua_state
//  The lua_State to push the object onto the stack of.
//
// @param position
//  The absolute position in the stack to get the object from (it is assumed
//  that this position is an absolute position; that is \e position > 0 or
//  position < LUA_REGISTRYINDEX (-10000)).
//
// @param type
//  The type to check that the object is (passed to lua_validate_type()).
//
// @return
//  The address of the object or null if there is no value at that position
//  on the stack or if the value at that position is nil.
*/
void* lua_to_object( lua_State* lua_state, int position, const rtti::Type& type )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( position > 0 || position < LUA_REGISTRYINDEX );

    void* object = NULL;
    if ( !lua_isnoneornil(lua_state, position) )
    {
        lua_validate_type( lua_state, position, type );
        lua_pushstring( lua_state, lua::THIS_KEYWORD );
        lua_rawget( lua_state, position );
        if ( lua_islightuserdata(lua_state, -1) )
        {
            object = lua_touserdata( lua_state, -1 );
            lua_pop( lua_state, 1 );
        }
        else if ( lua_isnil(lua_state, -1) )
        {
            lua_pushfstring( lua_state, "Missing this pointer for Lua object at position %d (not set or C++ object has been destroyed)", position );
            lua_error( lua_state );
        }
        else
        {
            lua_pushfstring( lua_state, "Invalid this pointer for Lua object at position %d (type=%s)", position, lua_typename(lua_state, lua_type(lua_state, position)) );
            lua_error( lua_state );
        }
    }
    return object;
}

/**
// @internal
//
// Create a LuaRawWrapper that causes a raw function to be generated instead 
// of a normal function.
//
// @param function
//  The C++ function to generate a raw function from.
//
// @return
//  A LuaRawWrapper that generates a raw function when passed to 
//  AddGlobal::operator() or AddMember::operator().
*/
LuaRawWrapper raw( lua_CFunction function )
{
    return LuaRawWrapper( function );
}

}

}
