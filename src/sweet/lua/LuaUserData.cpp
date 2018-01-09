//
// LuaUserData.cpp
// Copyright (c) 2008  - 2010 Charles Baker.  All rights reserved.
//

#include "LuaUserData.hpp"

using namespace sweet;
using namespace sweet::lua;

/**
// Constructor.
//
// @param type
//  The rtti::Type of the user's data stored in this LuaUserData.
*/
LuaUserData::LuaUserData( const rtti::Type& type )
: type_( type )
{
}

/**
// Copy constructor.
//
// @param user_data
//  The LuaUserData to copy.
*/
LuaUserData::LuaUserData( const LuaUserData& user_data )
: type_( user_data.type_ )
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
LuaUserData& LuaUserData::operator=( const LuaUserData& user_data )
{
    if ( this != &user_data )
    {
        type_ = user_data.type_;
    }

    return *this;
}

/**
// Get the rtti::Type of this LuaUserData.
//
// @return
//  The rtti::Type.
*/
const rtti::Type& LuaUserData::type() const
{
    return type_;
}
