//
// TestLuaValue.cpp
// Copyright (c) 2011 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaValue.hpp>

using namespace sweet::lua;

SUITE( LuaValue )
{
    TEST( LuaValueLeavesValueOnStack )
    {
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_newtable( lua_state );
        LuaValue value( lua, -1 );
        CHECK( lua_istable(lua_state, -1) );
    }

    TEST( LuaValueReferencesString )
    {
        const char* STRING_VALUE = "string";
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_pushstring( lua_state, STRING_VALUE );
        LuaValue value( lua, -1 );
        lua_pop( lua_state, 1 );
        lua_gc( lua_state, LUA_GCCOLLECT, 0 );
        lua_push( lua_state, value );
        CHECK( lua_isstring(lua_state, -1) );
        CHECK_EQUAL( lua_tostring(lua_state, -1), STRING_VALUE );
    }

    TEST( LuaValueReferencesTable )
    {
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_newtable( lua_state );
        LuaValue value( lua, -1 );
        lua_pop( lua_state, 1 );
        lua_gc( lua_state, LUA_GCCOLLECT, 0 );
        lua_push( lua_state, value );
        CHECK( lua_istable(lua_state, -1) );
    }

    TEST( LuaValueCanBeCopied )
    {
        const char* STRING_VALUE = "string";
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_pushstring( lua_state, STRING_VALUE );
        LuaValue value( lua, -1 );
        lua_pop( lua_state, 1 );
        LuaValue other_value( value );
        lua_push( lua_state, value );
        lua_push( lua_state, other_value );
        CHECK( lua_equal(lua_state, -1, -2) );
    }
    
    TEST( DefaultConstructedLuaValueCanBeAssignedTo )
    {
        const char* STRING_VALUE = "string";
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_pushstring( lua_state, STRING_VALUE );
        LuaValue value( lua, -1 );
        lua_pop( lua_state, 1 );
        LuaValue other_value;
        other_value = value;
        lua_push( lua_state, value );
        lua_push( lua_state, other_value );
        CHECK( lua_equal(lua_state, -1, -2) );
    }
    
    TEST( LuaValueCanBeAssignedTo )
    {
        const char* STRING_VALUE = "string";
        Lua lua;
        lua_State* lua_state = lua.get_lua_state();
        lua_pushstring( lua_state, STRING_VALUE );
        LuaValue value( lua, -1 );
        lua_pop( lua_state, 1 );
        LuaValue other_value( value );
        other_value = value;
        lua_push( lua_state, value );
        lua_push( lua_state, other_value );
        CHECK( lua_equal(lua_state, -1, -2) );
    }
}
