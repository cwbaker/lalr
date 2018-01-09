//
// LuaUserData.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAUSERDATA_IPP_INCLUDED
#define SWEET_LUA_LUAUSERDATA_IPP_INCLUDED

#include "LuaUserDataTemplate.hpp"

namespace sweet
{

namespace lua
{

/**
// Constructor.
//
// @param type
//  The rtti::Type of the user's data stored in this LuaUserData.
//
// @param value
//  The value of the user's data stored in this LuaUserData.
*/
template <class UserType>
LuaUserDataTemplate<UserType>::LuaUserDataTemplate( const rtti::Type& type, typename traits::traits<UserType>::parameter_type value )
: LuaUserData( type ),
  value_( value )
{
}

/**
// Copy constructor.
//
// @param user_data
//  The LuaUserData to copy.
*/
template <class UserType>
LuaUserDataTemplate<UserType>::LuaUserDataTemplate( const LuaUserDataTemplate& user_data )
: LuaUserData( user_data.type_ ),
  value_( user_data.value_ )
{
}

/**
// Assignment operator.
//
// @param user_data
//  The LuaUserData to copy from.
//
// @return
//  This LuaUserData.
*/
template <class UserType>
LuaUserDataTemplate<UserType>& 
LuaUserDataTemplate<UserType>::operator=( const LuaUserDataTemplate& user_data )
{
    if ( this != &user_data )
    {
        LuaUserData::operator=( *this );
        value_ = user_data.value_;
    }

    return *this;
}

/**
// Get the value of this LuaUserData.
//
// @return
//  The value.
*/
template <class UserType>
typename sweet::traits::traits<UserType>::reference_type 
LuaUserDataTemplate<UserType>::value()
{
    return value_;
}

}

}

#endif
