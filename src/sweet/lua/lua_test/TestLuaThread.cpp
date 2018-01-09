//
// TestLuaObject.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <unit/UnitTest.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/LuaThread.hpp>
#include <sweet/error/ErrorPolicy.hpp>

using namespace sweet;
using namespace sweet::lua;

SUITE( TestLuaThread )
{
    struct Fixture
    {
        error::ErrorPolicy error_policy;
        Lua lua;
        LuaThread lua_thread;
        LuaObject lua_object;

        Fixture()
        : error_policy(),
          lua( error_policy ),
          lua_thread( lua ),
          lua_object( lua )
        {
            lua.globals()
                ( "lua_object", lua_object )
                ( "yield", yield(&Fixture::yield_function) )
            ;
        }

        static void yield_function() {};
    };

    TEST_FIXTURE( Fixture, TestImport )
    {
        const char SCRIPT[] = "function add( a, b ) return a + b; end";
        lua_thread.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestImport" ).end();

        int result = 0;
        lua_thread.call( "add" )
            ( 1 )( 2 )
        .end( &result );
        CHECK( result == 1 + 2 );
    }

    TEST_FIXTURE( Fixture, TestCallFunction )
    {
        const char SCRIPT[] = "function bool_function() return true; end;";
        lua_thread.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestCallFunction" ).end();

        bool result = false;
        lua_thread.call( "bool_function" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestCallMemberFunction )
    {
        const char SCRIPT[] = "function lua_object:bool_function() return true; end;";
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestCallMemberFunction" ).end();

        bool result = false;
        lua_thread.call<LuaObject>( "bool_function", lua_object ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestResumeFunction )
    {
        const char SCRIPT[] = "function resume_function() yield(); yield(); return true; end;";
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestResumeFunction" ).end();

        lua_thread.resume( "resume_function" ).end();
        CHECK( lua_thread.get_state() == LUA_THREAD_SUSPENDED );

        lua_thread.resume().end();
        CHECK( lua_thread.get_state() == LUA_THREAD_SUSPENDED );

        bool result = false;
        lua_thread.resume().end( &result );
        CHECK( result );        
    }

    TEST_FIXTURE( Fixture, TestResumeMemberFunction )
    {
        const char SCRIPT[] = "function lua_object:resume_function() yield(); yield(); return true; end;";
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestResumeFunction" ).end();

        lua_thread.resume<LuaObject>( "resume_function", lua_object ).end();
        CHECK( lua_thread.get_state() == LUA_THREAD_SUSPENDED );

        lua_thread.resume().end();
        CHECK( lua_thread.get_state() == LUA_THREAD_SUSPENDED );

        bool result = false;
        lua_thread.resume().end( &result );
        CHECK( result );        
    }
}
