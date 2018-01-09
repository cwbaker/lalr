//
// AddMemberHelper.cpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#include "stdafx.hpp"
#include "lua_types.hpp"
#include "AddGlobal.hpp"
#include "AddParameter.hpp"
#include "AddMember.hpp"
#include "AddMemberHelper.hpp"
#include "AddParameterHelper.hpp"
#include "Lua.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lua;

AddMemberHelper::AddMemberHelper( Lua* lua )
: lua_( lua ),
  references_( 0 ),
  restore_to_position_( 0 )
{
    SWEET_ASSERT( lua_ );
}

lua_State* AddMemberHelper::get_lua_state() const
{
    SWEET_ASSERT( lua_ );
    return lua_->get_lua_state();
}

void AddMemberHelper::reference()
{
    if ( references_ == 0 )
    {
        SWEET_ASSERT( lua_ );
        restore_to_position_ = lua_gettop( lua_->get_lua_state() ) - 1;
    }

    ++references_;    
}

void AddMemberHelper::release()
{
    SWEET_ASSERT( references_ > 0 );

    --references_;
    if ( references_ == 0 )
    {
        SWEET_ASSERT( lua_ );
        lua_settop( lua_->get_lua_state(), restore_to_position_ );
        restore_to_position_ = 0;
    }
}
