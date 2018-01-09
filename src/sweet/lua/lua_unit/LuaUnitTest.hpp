
#ifndef SWEET_LUA_LUAUNITTEST_HPP_INCLUDED
#define SWEET_LUA_LUAUNITTEST_HPP_INCLUDED

#include "declspec.hpp"

struct lua_State;

namespace sweet
{

namespace lua
{

class Lua;

/**
// Provide Lua bindings to Noel Llopis's UnitTest++.
*/
class SWEET_LUA_UNIT_DECLSPEC LuaUnitTest
{
    Lua& lua_;

public:
    LuaUnitTest( Lua& lua );
    void run_tests( const char* filename );
    
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