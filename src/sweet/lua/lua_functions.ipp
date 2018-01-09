//
// lua_functions.ipp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_FUNCTIONS_IPP_INCLUDED
#define SWEET_LUA_FUNCTIONS_IPP_INCLUDED

#include "LuaConverter.hpp"
#include "LuaObjectConverter.hpp"
#include "LuaTraits.hpp"
#include "LuaUserDataTemplate.ipp"
#include <sweet/assert/assert.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Push a C++ object onto the stack by reference.
//
// @param lua
//  The lua_State to push the object onto the stack of.
//
// @param value
//  The object to push.
*/
template <class Type> 
void lua_push( lua_State* lua_state, Type* value )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<Type*, typename LuaTraits<base_type>::storage_type>::push( lua_state, value );
}

/**
// @internal
//
// Push a C++ object onto the stack by reference.
//
// @param lua
//  The lua_State to push the object onto the stack of.
//
// @param value
//  The object to push.
*/
template <class Type> 
void lua_push( lua_State* lua_state, const Type* value )
{
    typedef typename traits::traits<Type>::base_type base_type;
    LuaObjectConverter<const Type*, typename LuaTraits<base_type>::storage_type>::push( lua_state, value );
}

/**
// @internal
//
// Push a C++ object onto the stack by value.
//
// Copies \e value into a new object allocated as Lua userdata and sets the
// class's destructor to be called when the userdata value is garbage 
// collected (note that this may be some time after the userdata value is 
// no longer referenced).
//
// @param lua
//  The lua_State to push the object onto the stack of.
//
// @param value
//  The object to push.
*/
template <class Type> 
void lua_push_value( lua_State* lua_state, typename traits::traits<Type>::parameter_type value )
{
    typedef typename traits::traits<Type>::value_type value_type;
    SWEET_ASSERT( lua_state );
    void* copied_value = lua_newuserdata( lua_state, sizeof(LuaUserDataTemplate<value_type>) );
    lua_newtable( lua_state );
    lua_pushcfunction( lua_state, &lua_gc<LuaUserDataTemplate<value_type> > );
    lua_setfield( lua_state, -2, "__gc" );
    lua_setmetatable( lua_state, -2 );
    SWEET_ASSERT( copied_value != NULL );
    new (copied_value) LuaUserDataTemplate<value_type>( SWEET_TYPEID(value), value );
}

/**
// @internal
//
// Get a C++ object from the Lua stack.
//
// Returns a reference to the object stored on the Lua stack at \e position.
// The object on the stack is assumed to be Lua userdata and to be a C++ 
// object of the correct type.
//
// @param lua
//  The lua_State to get the object from.
//
// @param position
//  The position on the stack to get the value from.
//
// @return
//  A reference to the object at \e position.
*/
template <class Type> 
typename sweet::traits::traits<Type>::reference_type
lua_to_value( lua_State* lua_state, int position )
{
    SWEET_ASSERT( lua_state != 0 );
    SWEET_ASSERT( lua_isuserdata(lua_state, position) );
    SWEET_ASSERT( lua_touserdata(lua_state, position) != 0 );
    LuaUserDataTemplate<Type>* user_data = static_cast<LuaUserDataTemplate<Type>*>( lua_touserdata(lua_state, position) );
    SWEET_ASSERT( user_data != NULL );
    return user_data->value();
}

/**
// @internal
//
// Force a value to be pushed onto the Lua stack by value.
//
// @param lua
//  The lua_State to push the object onto the stack of.
//
// @param value
//  The value to push.
*/
template <class Type> 
LuaValueWrapper<Type> value( Type value )
{
    return LuaValueWrapper<Type>( value );
}

/**
// @internal
//
// Create a LuaPolicyWrapper that causes a yielding function to be generated
// instead of a normal function.
//
// @param function
//  The C++ function to generate a yielding function from.
//
// @return
//  A LuaPolicyWrapper that generates a yielding function when passed to 
//  AddGlobal::operator() or AddMember::operator().
*/
template <class Function> 
LuaPolicyWrapper<Function, sweet::lua::LUA_POLICY_YIELD> 
yield( Function function )
{
    return LuaPolicyWrapper<Function, LUA_POLICY_YIELD>( function );
}


/**
// @internal
//
// Create a LuaPolicyWrapper that causes a weakening function to be generated
// instead of a normal function.
//
// A weakening function takes its return value and moves it from the 
*/
template <class Function>
LuaPolicyWrapper<Function, sweet::lua::LUA_POLICY_WEAKEN>
weaken( Function function )
{
    return LuaPolicyWrapper<Function, LUA_POLICY_WEAKEN>( function );
}

/**
// @internal
//
// Garbage collect the user data allocated for an arbitrary C++ object.
//
// This calls the in place destructor for the object.
//
// @param lua_state
//  The lua_State that is garbage collecting the object.
// 
// @return
//  Always returns 0.
*/
template <class Object> 
int lua_gc( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( lua_isuserdata(lua_state, 1) );
    Object* object = static_cast<Object*>( lua_touserdata(lua_state, 1) );
    SWEET_ASSERT( object != NULL );
    object->Object::~Object();
    lua_pop( lua_state, 1 );
    return 0;
}

/**
// @internal
//
// Convert a C++ standard library sequence defined by a pair of iterators 
// into a Lua iterator.
//
// This is intended to be used as a closure that has the pair of iterators
// stored in user data bound as up values.
//
// @param lua
//  The lua_State.
// 
// @return
//  Always returns 1.
*/
template <class Iterator>
int lua_iterator( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

//
// Get the iterator for the current position.
//
    SWEET_ASSERT( lua_isuserdata(lua_state, lua_upvalueindex(1)) );
    Iterator& iterator = LuaConverter<Iterator&>::to( lua_state, lua_upvalueindex(1) );

//
// Get the iterator for the position that is one past the end of the sequence
// to iterate over.
//
    SWEET_ASSERT( lua_isuserdata(lua_state, lua_upvalueindex(2)) );
    Iterator end = LuaConverter<Iterator>::to( lua_state, lua_upvalueindex(2) );

//
// If the current iterator is not at the end then push the current value and
// iterate to the next position.  Otherwise push nil to indicate that the end
// of the sequence has been reached.
//
    if ( iterator != end )
    {
        typedef typename Iterator::value_type value_type;
        LuaConverter<value_type>::push( lua_state, *iterator );
        ++iterator;
    }
    else
    {
        lua_pushnil( lua_state );
    }

    return 1;
}

template <class Iterator, class Function> 
int lua_iterator_with_function( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( lua_isuserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( lua_isuserdata(lua_state, lua_upvalueindex(2)) );
    SWEET_ASSERT( lua_isuserdata(lua_state, lua_upvalueindex(3)) );

    Iterator& iterator = LuaConverter<Iterator&>::to( lua_state, lua_upvalueindex(1) );
    Iterator end = LuaConverter<Iterator>::to( lua_state, lua_upvalueindex(2) );
    Function& function = LuaConverter<Function&>::to( lua_state, lua_upvalueindex(3) );
    while ( iterator != end && !function(*iterator) )
    {
        ++iterator;
    }
    
    if ( iterator != end )
    {
        typedef typename Iterator::value_type value_type;
        LuaConverter<value_type>::push( lua_state, *iterator );
        ++iterator;
    }
    else
    {
        lua_pushnil( lua_state );
    }

    return 1;
}

/**
// @internal
//
// Create a Lua iterator to iterate over the range [\e start, \e finish).
//
// Copies of the iterators \e start and \e finish are made by calling the in 
// place copy constructor on memory allocated as Lua user data.  A garbage 
// collection method is set for the metatable for the user data so that 
// the iterators are destroyed correctly when the user data is garbage 
// collected.
//
// The user data values containing the copied iterators are then bound as 
// up values to an iterator function (see lua_iterator()) that iterates
// over the range returning nil when the end is reached.
//
// @param lua_state
//  The lua_State.
// 
// @param start
//  The iterator at the position to being iterating over.
//
// @param finish
//  The iterator one position past the finish element to iterate over.
*/
template <class Iterator>
void lua_push_iterator( lua_State* lua_state, Iterator start, Iterator finish )
{
    SWEET_ASSERT( lua_state );
    lua_push_value<Iterator>( lua_state, start );
    lua_push_value<Iterator>( lua_state, finish );
    lua_pushcclosure( lua_state, &lua_iterator<Iterator>, 2 );
}

template <class Iterator, class Function> 
void lua_push_iterator( lua_State* lua_state, Iterator start, Iterator finish, const Function& function )
{
    SWEET_ASSERT( lua_state );
    lua_push_value<Iterator>( lua_state, start );
    lua_push_value<Iterator>( lua_state, finish );
    lua_push_value<Function>( lua_state, function );
    lua_pushcclosure( lua_state, &lua_iterator_with_function<Iterator, Function>, 3 );
}

}

}

#endif
