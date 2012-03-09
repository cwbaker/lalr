
#include "LuaUnitTest.hpp"
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/Error.hpp>
#include <sweet/lua/lua_/lua.h>
#include <sweet/unit/TestDetails.h>
#include <sweet/unit/TestResults.h>
#include <sweet/unit/TestReporter.h>
#include <sweet/unit/Checks.h>
#include <sweet/unit/MemoryOutStream.h>
#include <sweet/unit/Win32/TimeHelpers.h>
#include <sweet/unit/CurrentTest.h>
#include <string>

using std::string;
using namespace UnitTest;
using namespace sweet;
using namespace sweet::lua;
using namespace sweet::sheet;

LuaUnitTest::LuaUnitTest( UnitTest::TestReporter* reporter )
: reporter_( reporter )
{
    SWEET_ASSERT( reporter_ );
}

int LuaUnitTest::run_all_tests( const char* filename )
{
    SWEET_ASSERT( filename );

    lua::Lua lua;
    lua.globals()
        ( "TestSuite", raw(&LuaUnitTest::test_suite), this )
        ( "CHECK", raw(&LuaUnitTest::check) )
        ( "CHECK_EQUAL", raw(&LuaUnitTest::check_equal) )
        ( "CHECK_CLOSE", raw(&LuaUnitTest::check_close) )
    ;
    
    TestResults results( reporter_ );
    CurrentTest::Results() = &results;

    Timer timer;
    timer.Start();
    lua.set_stack_trace_enabled( true );
    lua.call( filename, filename ).end();
    const float total_time = timer.GetTimeInMs() / 1000.0f;
    reporter_->ReportSummary( results.GetTotalTestCount(), results.GetFailedTestCount(), results.GetFailureCount(), total_time );

    CurrentTest::Results() = NULL;
    return results.GetFailureCount();
}

int LuaUnitTest::test_suite( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    SWEET_ASSERT( CurrentTest::Results() );
    
    const int TEST_SUITE = 1;
    
    if ( !lua_istable(lua_state, TEST_SUITE) )
    {
        // @todo: Should LuaUnitTest::test_suite() throw a C++ exception 
        // when it is passed an invalid parameter instead of calling
        // lua_error()?
        lua_pushfstring( lua_state, "TestSuite expects a table not a '%s' as its first parameter", lua_typename(lua_state, lua_type(lua_state, TEST_SUITE)) );
        lua_error( lua_state );
    }

    SWEET_ASSERT( lua_islightuserdata(lua_state, lua_upvalueindex(1)) );
    LuaUnitTest* unit_test = reinterpret_cast<LuaUnitTest*>( lua_touserdata(lua_state, lua_upvalueindex(1)) );
    SWEET_ASSERT( unit_test );

    lua_getfield( lua_state, TEST_SUITE, "name" );    
    string suite_name = lua_isstring( lua_state, -1 ) ? lua_tostring( lua_state, -1 ) : "??";
    lua_pop( lua_state, 1 );
    
    lua_pushnil( lua_state );
    while ( lua_next(lua_state, TEST_SUITE) != 0 )
    {
        if ( lua_isfunction(lua_state, -1) && lua_isstring(lua_state, -2) )
        {
            lua_pushvalue( lua_state, -1 );
            lua_Debug debug;
            lua_getinfo( lua_state, ">S", &debug );
            string file = debug.source;
            int line = debug.linedefined;

            string test_name = lua_tostring( lua_state, -2 );
            UnitTest::TestDetails details( test_name.c_str(), suite_name.c_str(), file.c_str(), line );
	        UnitTest::CurrentTest::Details() = &details;

	        Timer test_timer;
	        test_timer.Start();
	        CurrentTest::Results()->OnTestStart( details );
            
            int result = lua_pcall( lua_state, 0, 0, 0 );
            switch ( result )
            {
                case 0:
                    break;

                case LUA_ERRRUN:
                {
                    MemoryOutStream stream;
                    stream << lua_tostring( lua_state, -1 );
                    CurrentTest::Results()->OnTestFailure( details, stream.GetText() );
                    lua_pop( lua_state, 1 );
                    break;
                }

                case LUA_ERRMEM:
                {
                    MemoryOutStream stream;
                    stream << "Out of memory - " << lua_tostring( lua_state, -1 );
                    CurrentTest::Results()->OnTestFailure( details, stream.GetText() );
                    lua_pop( lua_state, 1 );
                    break;
                }

                case LUA_ERRERR:
                {
                    MemoryOutStream stream;
                    stream << "Error handler failed - " << lua_tostring( lua_state, -1 );
                    CurrentTest::Results()->OnTestFailure( details, stream.GetText() );                    
                    lua_pop( lua_state, 1 );
                    break;
                }

                case -1:
                {
                    MemoryOutStream stream;
                    stream << "Execution failed due to an unhandled C++ exception";
                    CurrentTest::Results()->OnTestFailure( details, stream.GetText() );
                    break;
                }

                default:
                {
                    SWEET_ASSERT( false );
                    MemoryOutStream stream;
                    stream << "Execution failed in an unexpected way - " << lua_tostring( lua_state, -1 );
                    CurrentTest::Results()->OnTestFailure( details, stream.GetText() );
                    lua_pop( lua_state, 1 );
                    break;
                }
            }

            const int MAXIMUM_TIME = 1000;
	        int const time = test_timer.GetTimeInMs();
	        if ( time > MAXIMUM_TIME )
	        {
	            MemoryOutStream stream;
	            stream << "Global time constraint failed. Expected under " << MAXIMUM_TIME << "ms but took " << time << "ms.";
	            CurrentTest::Results()->OnTestFailure( details, stream.GetText() );
	        }
	        CurrentTest::Results()->OnTestFinish( details, static_cast<float>(time) / 1000.0f );
	        CurrentTest::Details() = NULL;
        }
        else
        {
            lua_pop( lua_state, 1 );
        }
    }

    return 0;
}

int LuaUnitTest::check( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );

    const int EXPRESSION = 1;
    const int MESSAGE = 2;

    bool expression = lua_toboolean( lua_state, EXPRESSION );
    if ( !expression )
    {
        string message = lua_isstring( lua_state, MESSAGE ) ? lua_tostring( lua_state, MESSAGE ) : string();
        UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)), message.c_str() );
    }
    
    return 0;
}

int LuaUnitTest::check_equal( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    
    const int EXPECTED = 1;
    const int ACTUAL = 2;
    
    switch ( lua_type(lua_state, 1) )
    {
        case LUA_TBOOLEAN:
        {
            bool expected = lua_toboolean( lua_state, EXPECTED );
            bool actual = lua_toboolean( lua_state, ACTUAL );
            UnitTest::CheckEqual( *UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)) );
            break;
        }
        
        case LUA_TNUMBER:
        {
            int expected = lua_tointeger( lua_state, EXPECTED );
            int actual = lua_tointeger( lua_state, ACTUAL );
            UnitTest::CheckEqual( *UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)) );
            break;
        }

        case LUA_TSTRING:
        {
            const char* expected = lua_tostring( lua_state, EXPECTED );
            const char* actual = lua_tostring( lua_state, ACTUAL );
            UnitTest::CheckEqual( *UnitTest::CurrentTest::Results(), expected, actual, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)) );
            break;
        }

        case LUA_TNIL:
        case LUA_TLIGHTUSERDATA:
        case LUA_TTABLE:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
            UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)), "CHECK_EQUAL() is only valid on booleans, numbers, and strings" );
            break;
        
        default:
        {
            SWEET_ASSERT( false );
            char message [256];
            _snprintf( message, sizeof(message), "CHECK_EQUAL() passed an unexpected type, type=%s '%s'", lua_type(lua_state, EXPECTED), lua_typename(lua_state, lua_type(lua_state, EXPECTED)) );
            UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)), message );
            break;
        }
    }    
    return 0;
}

int LuaUnitTest::check_close( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    
    const int EXPECTED = 1;
    const int ACTUAL = 2;
    const int TOLERANCE = 3;
    
    switch ( lua_type(lua_state, 1) )
    {        
        case LUA_TNUMBER:
        {
            float expected = static_cast<float>( lua_tonumber( lua_state, EXPECTED) );
            float actual = static_cast<float>( lua_tonumber( lua_state, ACTUAL) );
            float tolerance = static_cast<float>( lua_tonumber(lua_state, TOLERANCE) );
            UnitTest::CheckClose( *UnitTest::CurrentTest::Results(), expected, actual, tolerance, UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)) );
            break;
        }

        case LUA_TNIL:
        case LUA_TLIGHTUSERDATA:
        case LUA_TBOOLEAN:
        case LUA_TSTRING:
        case LUA_TTABLE:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
            UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)), "CHECK_CLOSE() is only valid on numbers" );
            break;
        
        default:
            SWEET_ASSERT( false );
            char message [256];
            _snprintf( message, sizeof(message), "CHECK_CLOSE() passed an unexpected type, type=%s '%s'", lua_type(lua_state, EXPECTED), lua_typename(lua_state, lua_type(lua_state, EXPECTED)) );
            UnitTest::CurrentTest::Results()->OnTestFailure( UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), LuaUnitTest::line(lua_state)), message );
            break;
    }
    
    return 0;
}

int LuaUnitTest::line( lua_State* lua_state )
{
    lua_Debug debug;
    lua_getstack( lua_state, 1, &debug );
    lua_getinfo( lua_state, "l", &debug );
    return debug.currentline;
}
