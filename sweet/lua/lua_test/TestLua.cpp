//
// TestLua.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/Error.hpp>
#include <float.h>

using namespace sweet::lua;

SUITE( TestLua )
{
    struct Fixture
    {
        Lua lua;

        static void void_function() {};
        static bool bool_function() { return true; };
        static int int_function() { return 0; };
        static float float_function() { return 0.0f; };
        static std::string string_function() { return "foo"; };
        static int raw_function( lua_State* lua_state ) { SWEET_ASSERT(lua_state != NULL); return 0; };

        static bool function_bool( bool a, bool b ) { return a && b; };
        static int function_int( int a, int b ) { return a + b; };
        static float function_float( float a, float b ) { return a + b; };
        static std::string function_string( const std::string& a, const std::string& b ) { return a + b; };

        static void function_1( int ) {};
        static void function_2( int, int ) {};
        static void function_3( int, int, int ) {};
        static void function_4( int, int, int, int ) {};
        static void function_5( int, int, int, int, int ) {};
        static void function_6( int, int, int, int, int, int ) {};
    };

    TEST_FIXTURE( Fixture, TestGetLuaState )
    {
        CHECK( lua.get_lua_state() );
    }

    TEST_FIXTURE( Fixture, TestImport )
    {
        const char SCRIPT[] = "function add( a, b ) return a + b; end";
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestImport" ).end();

        int result = 0;
        lua.call( "add" )
            ( 1 )( 2 )
        .end( &result );
        CHECK( result == 1 + 2 );
    }

    TEST_FIXTURE( Fixture, TestCallMemberFunction )
    {
        LuaObject lua_object( lua );
        lua.globals()
            ( "Foo", lua_object )
        ;

        const char SCRIPT[] = "function Foo:add( a, b ) return a + b; end";
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestCallMemberFunction").end();
        
        int result = 0;
        lua.call( "add", lua_object )
            ( lua_object )
            ( 1 )
            ( 2 )
        .end( &result );
        CHECK( result == 1 + 2 );
    }

    TEST_FIXTURE( Fixture, TestAddNil )
    {
        lua.globals()
            ( "nil", nil )
        ;

        CHECK( !lua.is_value("nil") );
    }

    TEST_FIXTURE( Fixture, TestAddGlobalEnvironment )
    {
        lua.globals()
            ( "global_environment", global_environment )
        ;

        CHECK( lua.is_value("global_environment") );
    }

    TEST_FIXTURE( Fixture, TestAddBool )
    {
        lua.globals()
            ( "true_boolean", true )
            ( "false_boolean", false )
        ;
        
        CHECK( lua.is_value("true_boolean") );
        CHECK( lua.boolean("true_boolean") );
        CHECK( lua.is_value("false_boolean") );
        CHECK( !lua.boolean("false_boolean") );
    }

    TEST_FIXTURE( Fixture, TestAddInt )
    {
        lua.globals() 
            ( "a", 0     )
            ( "b", 8     )
            ( "c", 32768 )
            ( "d", 65536 )
            ( "e", -1    )
        ;

        CHECK( lua.is_value("a") );
        CHECK( lua.integer("a") == 0 );
        CHECK( lua.is_value("b") );
        CHECK( lua.integer("b") == 8 );
        CHECK( lua.is_value("c") );
        CHECK( lua.integer("c") == 32768 );
        CHECK( lua.is_value("d") );
        CHECK( lua.integer("d") == 65536 );
        CHECK( lua.is_value("e") );
        CHECK( lua.integer("e") == -1 );
    }

    TEST_FIXTURE( Fixture, TestAddFloat )
    {
        lua.globals()
            ( "a", 0.0f     )
            ( "b", FLT_MAX  )
            ( "c", -FLT_MAX )
            ( "d", FLT_MIN  )
            ( "e", -FLT_MIN )
        ;

        CHECK( lua.is_value("a") );
        CHECK( lua.number("a") == 0.0f );
        CHECK( lua.is_value("b") );
        CHECK( lua.number("b") == FLT_MAX );
        CHECK( lua.is_value("c") );
        CHECK( lua.number("c") == -FLT_MAX );
        CHECK( lua.is_value("d") );
        CHECK( lua.number("d") == FLT_MIN );
        CHECK( lua.is_value("e") );
        CHECK( lua.number("e") == -FLT_MIN );
    }

    TEST_FIXTURE( Fixture, TestAddString )
    {
        lua.globals()
            ( "a", "string string string"              )
            ( "b", std::string("gnirts gnirts gnirts") )
        ;

        CHECK( lua.is_value("a") );
        CHECK( lua.string("a") == "string string string" );
        CHECK( lua.is_value("b") );
        CHECK( lua.string("b") == "gnirts gnirts gnirts" );
    }
    
    TEST_FIXTURE( Fixture, TestAddLuaObject )
    {
        LuaObject object( lua );

        lua.globals()
            ( "object", object )
        ;

        CHECK( lua.is_value("object") );
    }

    TEST_FIXTURE( Fixture, TestAddCxxObject )
    {
        struct Foo
        {
            bool* destroyed_;

            Foo( bool* destroyed )
            : destroyed_( destroyed )
            {
            }

            ~Foo()
            {
                *destroyed_ = true;
            }
        };

        bool destroyed = false;
        Foo foo( &destroyed );

        lua.globals()( "foo", foo );
        CHECK( lua.is_value("foo") );

        lua.globals()( "foo", nil );
        CHECK( !lua.is_value("foo") );
        
        lua_gc( lua.get_lua_state(), LUA_GCCOLLECT, 0 );
        CHECK( destroyed );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionReturnTypes )
    {
        lua.globals()
            ( "void_function",   &Fixture::void_function   )
            ( "bool_function",   &Fixture::bool_function   )
            ( "int_function",    &Fixture::int_function    )
            ( "float_function",  &Fixture::float_function  )
            ( "string_function", &Fixture::string_function )
        ;

        CHECK( lua.is_function("void_function") );
        CHECK( lua.is_function("bool_function") );
        CHECK( lua.is_function("int_function") );
        CHECK( lua.is_function("float_function") );
        CHECK( lua.is_function("string_function") );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionWithParameters )
    {
        lua.globals()
            ( "function_1", &Fixture::function_1 )
            ( "function_2", &Fixture::function_2 )
            ( "function_3", &Fixture::function_3 )
            ( "function_4", &Fixture::function_4 )
            ( "function_5", &Fixture::function_5 )
            ( "function_6", &Fixture::function_6 )
        ;

        CHECK( lua.is_function("function_1") );
        CHECK( lua.is_function("function_2") );
        CHECK( lua.is_function("function_3") );
        CHECK( lua.is_function("function_4") );
        CHECK( lua.is_function("function_5") );
        CHECK( lua.is_function("function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionWithUpValues )
    {
        lua.globals()
            ( "function_1", &Fixture::function_1, 1 )
            ( "function_2", &Fixture::function_2, 1, 2 )
            ( "function_3", &Fixture::function_3, 1, 2, 3 )
            ( "function_4", &Fixture::function_4, 1, 2, 3, 4 )
            ( "function_5", &Fixture::function_5, 1, 2, 3, 4, 5 )
            ( "function_6", &Fixture::function_6, 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function("function_1") );
        CHECK( lua.is_function("function_2") );
        CHECK( lua.is_function("function_3") );
        CHECK( lua.is_function("function_4") );
        CHECK( lua.is_function("function_5") );
        CHECK( lua.is_function("function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddYieldingFunctionWithParameters )
    {
        lua.globals()
            ( "function_1", yield(&Fixture::function_1) )
            ( "function_2", yield(&Fixture::function_2) )
            ( "function_3", yield(&Fixture::function_3) )
            ( "function_4", yield(&Fixture::function_4) )
            ( "function_5", yield(&Fixture::function_5) )
            ( "function_6", yield(&Fixture::function_6) )
        ;

        CHECK( lua.is_function("function_1") );
        CHECK( lua.is_function("function_2") );
        CHECK( lua.is_function("function_3") );
        CHECK( lua.is_function("function_4") );
        CHECK( lua.is_function("function_5") );
        CHECK( lua.is_function("function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddYieldingFunctionWithUpValues )
    {
        lua.globals()
            ( "function_1", yield(&Fixture::function_1), 1 )
            ( "function_2", yield(&Fixture::function_2), 1, 2 )
            ( "function_3", yield(&Fixture::function_3), 1, 2, 3 )
            ( "function_4", yield(&Fixture::function_4), 1, 2, 3, 4 )
            ( "function_5", yield(&Fixture::function_5), 1, 2, 3, 4, 5 )
            ( "function_6", yield(&Fixture::function_6), 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function("function_1") );
        CHECK( lua.is_function("function_2") );
        CHECK( lua.is_function("function_3") );
        CHECK( lua.is_function("function_4") );
        CHECK( lua.is_function("function_5") );
        CHECK( lua.is_function("function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddRawFunction )
    {
        lua.globals()
            ( "raw_function", raw(&Fixture::raw_function) )
        ;

        CHECK( lua.is_function("raw_function") );
    }

    TEST_FIXTURE( Fixture, TestAddRawFunctionWithUpValues )
    {
        lua.globals()
            ( "raw_function_1", raw(&Fixture::raw_function), 1 )
            ( "raw_function_2", raw(&Fixture::raw_function), 1, 2 )
            ( "raw_function_3", raw(&Fixture::raw_function), 1, 2, 3 )
            ( "raw_function_4", raw(&Fixture::raw_function), 1, 2, 3, 4 )
            ( "raw_function_5", raw(&Fixture::raw_function), 1, 2, 3, 4, 5 )
            ( "raw_function_6", raw(&Fixture::raw_function), 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function("raw_function_1") );
        CHECK( lua.is_function("raw_function_2") );
        CHECK( lua.is_function("raw_function_3") );
        CHECK( lua.is_function("raw_function_4") );
        CHECK( lua.is_function("raw_function_5") );
        CHECK( lua.is_function("raw_function_6") );
    }

    TEST_FIXTURE( Fixture, TestFunctionBool )
    {
        lua.globals()
            ( "function_bool", &Fixture::function_bool )
        ;
        
        const char SCRIPT[] = "return function_bool(true, false) == false;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionBool").end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestFunctionInt )
    {
        lua.globals()
            ( "function_int", &Fixture::function_int )
        ;

        const char SCRIPT[] = "return function_int(1, 2) == 1 + 2;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionInt").end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestFunctionFloat )
    {
        lua.globals()
            ( "function_float", &Fixture::function_float )
        ;

        const char SCRIPT[] = "return function_float(1.0, 2.0) == 1.0 + 2.0;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionFloat").end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestFunctionString )
    {
        lua.globals()
            ( "function_string", &Fixture::function_string )
        ;

        const char SCRIPT[] = "return function_string(\"foo\", \"bar\") == \"foo\"..\"bar\";";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionString").end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestBoolTypeError )
    {
        lua.globals()
            ( "function_bool", &Fixture::function_bool )
        ;

        const char SCRIPT[] = "return function_bool({}, false);";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionBool").end(), RuntimeError );
    }

    TEST_FIXTURE( Fixture, TestIntTypeError )
    {
        lua.globals()
            ( "function_int", &Fixture::function_int )
        ;

        const char SCRIPT[] = "return function_int({}, 2);";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionInt").end(), RuntimeError );
    }

    TEST_FIXTURE( Fixture, TestFloatTypeError )
    {
        lua.globals()
            ( "function_float", &Fixture::function_float )
        ;

        const char SCRIPT[] = "return function_float({}, 2.0) == 1.0 + 2.0;";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionFloat").end(), RuntimeError );
    }

    TEST_FIXTURE( Fixture, TestStringTypeError )
    {
        lua.globals()
            ( "function_string", &Fixture::function_string )
        ;

        const char SCRIPT[] = "return function_string({}, \"bar\");";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionString").end(), RuntimeError );
    }
    
    TEST_FIXTURE( Fixture, AssertIsHandledByCxx )
    {
        const char SCRIPT[] = "assert( false, 'Assert in Lua script test' );";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "AssertIsHandledByCxx").end(), RuntimeError );
    }

    TEST_FIXTURE( Fixture, AssertWithoutErrorMessageIsHandledByCxx )
    {
        const char SCRIPT[] = "assert( false );";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "AssertWithoutErrorMessageIsHandledByCxx").end(), RuntimeError );
    }
}