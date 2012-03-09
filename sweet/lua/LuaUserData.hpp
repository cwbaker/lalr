//
// LuaUserData.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAUSERDATA_HPP_INCLUDED
#define SWEET_LUA_LUAUSERDATA_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/rtti/Type.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// The base class for arbitrary user data values annotated with information 
// about their type.
*/
class SWEET_LUA_DECLSPEC LuaUserData
{
    rtti::Type type_; ///< The Type of the user's data.

    public:
        LuaUserData( const rtti::Type& type );
        LuaUserData( const LuaUserData& user_data );
        LuaUserData& operator=( const LuaUserData& user_data );
        const rtti::Type& type() const;
};

}

}

#endif
