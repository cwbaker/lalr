//
// LuaUserDataTemplate.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAUSERDATATEMPLATE_HPP_INCLUDED
#define SWEET_LUA_LUAUSERDATATEMPLATE_HPP_INCLUDED

#include "LuaUserData.hpp"
#include <sweet/traits/traits.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// The implementation of an arbitrary user data value annotated with 
// information about its type.
*/
template <class UserType>
class LuaUserDataTemplate : public LuaUserData
{
    typename traits::traits<UserType>::value_type value_; ///< The value of the user's data.

    public:
        LuaUserDataTemplate( const rtti::Type& type, typename traits::traits<UserType>::parameter_type value );
        LuaUserDataTemplate( const LuaUserDataTemplate& user_data );
        LuaUserDataTemplate& operator=( const LuaUserDataTemplate& user_data );
        typename traits::traits<UserType>::reference_type value();
};

}

}

#endif
