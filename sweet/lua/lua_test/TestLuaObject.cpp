//
// TestLuaObject.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaObject.hpp>
#include <sweet/lua/LuaTraits.hpp>
#include <sweet/lua/Error.hpp>
#include <float.h>

using namespace sweet::lua;

struct Object
{
    void        void_function() {};
    bool        bool_function() { return false; };
    int         int_function() { return 0; };
    float       float_function() { return 0.0f; };
    std::string string_function() { return "foo"; };

    bool        function_bool( bool a ) { return a; };
    int         function_int( int a ) { return a; };
    float       function_float( float a ) { return a; };
    std::string function_string( const std::string& a ) { return a; };

    void function_1( int ) {};
    void function_2( int, int ) {};
    void function_3( int, int, int ) {};
    void function_4( int, int, int, int ) {};
    void function_5( int, int, int, int, int ) {};
    void function_6( int, int, int, int, int, int ) {};

    static int raw_function( lua_State* lua_state ) { SWEET_ASSERT(lua_state != NULL); return 0; };
};

SWEET_LUA_TYPE_CONVERSION( Object, LuaByReference );

SUITE( TestLuaObject )
{
    struct Fixture
    {
        Lua lua;
        Object object;

        Fixture()
        : lua(),
          object()
        {
            lua.create( object );
            lua.members( object )
                .type( SWEET_STATIC_TYPEID(Object) )
                .this_pointer( &object )
            ;
        }
    };

    TEST_FIXTURE( Fixture, TestAddThisPointer )
    {
        void* this_pointer = reinterpret_cast<void*>( 0xc0dedbad );

        lua.members( object )
            .this_pointer( this_pointer )
        ;

        CHECK( lua.is_value(object, THIS_KEYWORD) );        
    }

    TEST_FIXTURE( Fixture, TestAddType )
    {
        lua.members( object )
            .type( SWEET_STATIC_TYPEID(Fixture) )
        ;

        CHECK( lua.is_value(object, TYPE_KEYWORD) );
    }

    TEST_FIXTURE( Fixture, TestAddMetatable )
    {
        LuaObject metatable( lua );

        lua.members( object )
            .metatable( metatable )
        ;        
    }

    TEST_FIXTURE( Fixture, TestAddNil )
    {
        lua.members( object )
            ( "foo", 1 )
        ;
        CHECK( lua.is_value(object, "foo") );

        lua.members( object )
            ( "foo", nil )
        ;
        CHECK( !lua.is_value(object, "foo") );
    }

    TEST_FIXTURE( Fixture, TestAddGlobalEnvironment )
    {
        lua.members( object )
            ( "global_environment", global_environment )
        ;
        CHECK( lua.is_value(object, "global_environment") );
    }

    TEST_FIXTURE( Fixture, TestAddBool )
    {
        lua.members( object )
            ( "boolean", true )
        ;

        CHECK( lua.is_value(object, "boolean") );
        CHECK( lua.is_boolean(object, "boolean") );
        CHECK( lua.boolean(object, "boolean") == true );
    }

    TEST_FIXTURE( Fixture, TestAddInt )
    {
        lua.members( object )
            ( "integer", 1 )
        ;

        CHECK( lua.is_value(object, "integer") );
        CHECK( lua.is_number(object, "integer") );
        CHECK( lua.integer(object, "integer") == 1 );
    }

    TEST_FIXTURE( Fixture, TestAddFloat )
    {
        lua.members( object )
            ( "a", 0.0f     )
            ( "b", FLT_MAX  )
            ( "c", -FLT_MAX )
            ( "d", FLT_MIN  )
            ( "e", -FLT_MIN )
        ;

        CHECK( lua.is_value(object, "a") );
        CHECK( lua.number(object, "a") == 0.0f );
        CHECK( lua.is_value(object, "b") );
        CHECK( lua.number(object, "b") == FLT_MAX );
        CHECK( lua.is_value(object, "c") );
        CHECK( lua.number(object, "c") == -FLT_MAX );
        CHECK( lua.is_value(object, "d") );
        CHECK( lua.number(object, "d") == FLT_MIN );
        CHECK( lua.is_value(object, "e") );
        CHECK( lua.number(object, "e") == -FLT_MIN );
    }

    TEST_FIXTURE( Fixture, TestAddString )
    {
        lua.members( object )
            ( "a", "string string string"              )
            ( "b", std::string("gnirts gnirts gnirts") )
        ;

        CHECK( lua.is_value(object, "a") );
        CHECK( lua.string(object, "a") == "string string string" );
        CHECK( lua.is_value(object, "b") );
        CHECK( lua.string(object, "b") == "gnirts gnirts gnirts" );
    }

    TEST_FIXTURE( Fixture, TestAddLuaObject )
    {
        Object other_object;
        lua.create( other_object );

        lua.members( object )
            ( "other_object", other_object )
        ;

        CHECK( lua.is_value(object, "other_object") );
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

        lua.members( object )( "foo", foo );
        CHECK( lua.is_value(object, "foo") );

        lua.members( object )( "foo", nil );
        CHECK( !lua.is_value(object, "foo") );
        
        lua_gc( lua.get_lua_state(), LUA_GCCOLLECT, 0 );
        CHECK( destroyed );
    }

    TEST_FIXTURE( Fixture, TestVoidFunction )
    {
        lua.members( object )
            ( "void_function", &Object::void_function )
        ;
        CHECK( lua.is_function(object, "void_function") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "lua_object:void_function(); return true;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestVoidFunction" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestBoolFunction )
    {
        lua.members( object )
            ( "bool_function", &Object::bool_function )
        ;
        CHECK( lua.is_function(object, "bool_function") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:bool_function() == false;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestBoolFunction" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestIntFunction )
    {
        lua.members( object )
            ( "int_function", &Object::int_function )
        ;
        CHECK( lua.is_function(object, "int_function") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:int_function() == 0;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestIntFunction" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestFloatFunction )
    {
        lua.members( object )
            ( "float_function", &Object::float_function )
        ;
        CHECK( lua.is_function(object, "float_function") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:float_function() == 0.0;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFloatFunction" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestStringFunction )
    {
        lua.members( object )
            ( "string_function", &Object::string_function )
        ;
        CHECK( lua.is_function(object, "string_function") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:string_function() == \"foo\";";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestStringFunction" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestFunctionBool )
    {
        lua.members( object )
            ( "function_bool", &Object::function_bool )
        ;        
        CHECK( lua.is_function(object, "function_bool") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:function_bool( true ) == true;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionBool" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionInt )
    {
        lua.members( object )
            ( "function_int", &Object::function_int )
        ;        
        CHECK( lua.is_function(object, "function_int") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:function_int( 0 ) == 0;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionInt" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionFloat )
    {
        lua.members( object )
            ( "function_float", &Object::function_float )
        ;       
        CHECK( lua.is_function(object, "function_float") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:function_float( 0.0 ) == 0.0;";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionFloat" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionString )
    {
        lua.members( object )
            ( "function_string", &Object::function_string )
        ;        
        CHECK( lua.is_function(object, "function_string") );

        lua.globals()( "lua_object", object );
        CHECK( lua.is_value("lua_object") );

        const char SCRIPT[] = "return lua_object:function_string( \"bar\" ) == \"bar\";";
        bool result = false;
        lua.call( SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunctionString" ).end( &result );
        CHECK( result );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionWithParameters )
    {
        lua.members( object )
            ( "function_1", &Object::function_1 )
            ( "function_2", &Object::function_2 )
            ( "function_3", &Object::function_3 )
            ( "function_4", &Object::function_4 )
            ( "function_5", &Object::function_5 )
            ( "function_6", &Object::function_6 )
        ;

        CHECK( lua.is_function(object, "function_1") );
        CHECK( lua.is_function(object, "function_2") );
        CHECK( lua.is_function(object, "function_3") );
        CHECK( lua.is_function(object, "function_4") );
        CHECK( lua.is_function(object, "function_5") );
        CHECK( lua.is_function(object, "function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddFunctionWithUpValues )
    {
        lua.members( object )
            ( "function_1", &Object::function_1, 1 )
            ( "function_2", &Object::function_2, 1, 2 )
            ( "function_3", &Object::function_3, 1, 2, 3 )
            ( "function_4", &Object::function_4, 1, 2, 3, 4 )
            ( "function_5", &Object::function_5, 1, 2, 3, 4, 5 )
            ( "function_6", &Object::function_6, 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function(object, "function_1") );
        CHECK( lua.is_function(object, "function_2") );
        CHECK( lua.is_function(object, "function_3") );
        CHECK( lua.is_function(object, "function_4") );
        CHECK( lua.is_function(object, "function_5") );
        CHECK( lua.is_function(object, "function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddYieldingFunctionWithParameters )
    {
        lua.members( object )
            ( "function_1", yield(&Object::function_1) )
            ( "function_2", yield(&Object::function_2) )
            ( "function_3", yield(&Object::function_3) )
            ( "function_4", yield(&Object::function_4) )
            ( "function_5", yield(&Object::function_5) )
            ( "function_6", yield(&Object::function_6) )
        ;

        CHECK( lua.is_function(object, "function_1") );
        CHECK( lua.is_function(object, "function_2") );
        CHECK( lua.is_function(object, "function_3") );
        CHECK( lua.is_function(object, "function_4") );
        CHECK( lua.is_function(object, "function_5") );
        CHECK( lua.is_function(object, "function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddYieldingFunctionWithUpValues )
    {
        lua.members( object )
            ( "function_1", yield(&Object::function_1), 1 )
            ( "function_2", yield(&Object::function_2), 1, 2 )
            ( "function_3", yield(&Object::function_3), 1, 2, 3 )
            ( "function_4", yield(&Object::function_4), 1, 2, 3, 4 )
            ( "function_5", yield(&Object::function_5), 1, 2, 3, 4, 5 )
            ( "function_6", yield(&Object::function_6), 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function(object, "function_1") );
        CHECK( lua.is_function(object, "function_2") );
        CHECK( lua.is_function(object, "function_3") );
        CHECK( lua.is_function(object, "function_4") );
        CHECK( lua.is_function(object, "function_5") );
        CHECK( lua.is_function(object, "function_6") );
    }

    TEST_FIXTURE( Fixture, TestAddRawFunction )
    {
        lua.members( object )
            ( "raw_function", raw(&Object::raw_function) )
        ;

        CHECK( lua.is_function(object, "raw_function") );
    }

    TEST_FIXTURE( Fixture, TestAddRawFunctionWithUpValues )
    {
        lua.members( object )
            ( "raw_function_1", raw(&Object::raw_function), 1 )
            ( "raw_function_2", raw(&Object::raw_function), 1, 2 )
            ( "raw_function_3", raw(&Object::raw_function), 1, 2, 3 )
            ( "raw_function_4", raw(&Object::raw_function), 1, 2, 3, 4 )
            ( "raw_function_5", raw(&Object::raw_function), 1, 2, 3, 4, 5 )
            ( "raw_function_6", raw(&Object::raw_function), 1, 2, 3, 4, 5, 6 )
        ;

        CHECK( lua.is_function(object, "raw_function_1") );
        CHECK( lua.is_function(object, "raw_function_2") );
        CHECK( lua.is_function(object, "raw_function_3") );
        CHECK( lua.is_function(object, "raw_function_4") );
        CHECK( lua.is_function(object, "raw_function_5") );
        CHECK( lua.is_function(object, "raw_function_6") );
    }

    TEST_FIXTURE( Fixture, TestTypeError )
    {
        lua.members( object )
            .type( SWEET_STATIC_TYPEID(int) )
            ( "function_1", &Object::function_1 )
        ;

        const char SCRIPT[] = "lua_object:function_1( {} );";
        CHECK_THROW( lua.call(SCRIPT, SCRIPT + sizeof(SCRIPT) - 1, "TestFunction1").end(), RuntimeError );
    }
}