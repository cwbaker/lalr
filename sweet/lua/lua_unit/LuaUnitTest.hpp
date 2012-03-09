
#ifndef SWEET_LUA_LUAUNITTEST_HPP_INCLUDED
#define SWEET_LUA_LUAUNITTEST_HPP_INCLUDED

#include "declspec.hpp"

struct lua_State;

namespace UnitTest
{

class TestReporter;

}

namespace sweet
{

namespace sheet
{

/**
// Provide Lua bindings to Noel Llopis's UnitTest++.
*/
class SWEET_LUA_UNIT_DECLSPEC LuaUnitTest
{
    UnitTest::TestReporter* reporter_;

public:
    LuaUnitTest( UnitTest::TestReporter* reporter );
    int run_all_tests( const char* filename );
    
private:
    static int test_suite( lua_State* lua_state );
    static int check( lua_State* lua_state );
    static int check_equal( lua_State* lua_state );
    static int check_close( lua_State* lua_state );
    static int line( lua_State* lua_state );
};

}

}

#endif