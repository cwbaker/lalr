//
// AddMemberHelper.hpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDMEMBERHELPER_HPP_INCLUDED
#define SWEET_LUA_ADDMEMBERHELPER_HPP_INCLUDED

#include "declspec.hpp"

struct lua_State;

namespace sweet
{

namespace lua
{

class Lua;

class SWEET_LUA_DECLSPEC AddMemberHelper
{
    Lua* lua_;
    int references_;
    int restore_to_position_;
    
    public:
        AddMemberHelper( Lua* lua );   
        lua_State* get_lua_state() const;     
        void reference();
        void release();
};

}

}

#endif