//
// LuaOutOfOrderParameter.hpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAOUTOFORDERPARAMETER_HPP_INCLUDED
#define SWEET_LUA_LUAOUTOFORDERPARAMETER_HPP_INCLUDED

#include "declspec.hpp" 

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A type to bind Lua parameters to C++ functions in a different order to
// what they appear in the function signature.
*/
template <int POSITION>
class LuaOutOfOrderParameter
{
};

SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<1> _1;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<2> _2;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<3> _3;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<4> _4;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<5> _5;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<6> _6;
SWEET_LUA_DECLSPEC extern const LuaOutOfOrderParameter<7> _7;

}

}

#endif
