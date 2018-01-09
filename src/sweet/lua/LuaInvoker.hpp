//
// LuaInvoker.hpp
// Copyright (c) 2007 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAINVOKER_HPP_INCLUDED
#define SWEET_LUA_LUAINVOKER_HPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Call a C++ function and push its return value onto the Lua stack.
*/
template <class Returner, class Function>
struct LuaInvoker
{
};

template <class Returner, class R>
struct LuaInvoker<Returner, R (*)()>
{
    typedef R (*Function)();
    static int call( lua_State* lua, Function function );
};

template <class Returner>
struct LuaInvoker<Returner, void (*)()>
{
    typedef void (*Function)();
    static int call( lua_State* lua, Function function );
};

template <class Returner, class R, class A0>
struct LuaInvoker<Returner, R (*)(A0)>
{
    typedef R (*Function)(A0);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0 );
};

template <class Returner, class A0>
struct LuaInvoker<Returner, void (*)(A0)>
{
    typedef void (*Function)(A0);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0 );
};

template <class Returner, class R, class A0, class A1>
struct LuaInvoker<Returner, R (*)(A0, A1)>
{
    typedef R (*Function)(A0, A1);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class A0, class A1>
struct LuaInvoker<Returner, void (*)(A0, A1)>
{
    typedef void (*Function)(A0, A1);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class R, class A0, class A1, class A2>
struct LuaInvoker<Returner, R (*)(A0, A1, A2)>
{
    typedef R (*Function)(A0, A1, A2);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class A0, class A1, class A2>
struct LuaInvoker<Returner, void (*)(A0, A1, A2)>
{
    typedef void (*Function)(A0, A1, A2);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class R, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, R (*)(A0, A1, A2, A3)>
{
    typedef R (*Function)(A0, A1, A2, A3);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, void (*)(A0, A1, A2, A3)>
{
    typedef void (*Function)(A0, A1, A2, A3);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class R, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, R (*)(A0, A1, A2, A3, A4)>
{
    typedef R (*Function)(A0, A1, A2, A3, A4);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 );
};

template <class Returner, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, void (*)(A0, A1, A2, A3, A4)>
{
    typedef void (*Function)(A0, A1, A2, A3, A4);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a5 );
};

template <class Returner, class R, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, R (*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R (*Function)(A0, A1, A2, A3, A4, A5);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

template <class Returner, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, void (*)(A0, A1, A2, A3, A4, A5)>
{
    typedef void (*Function)(A0, A1, A2, A3, A4, A5);
    static int call( lua_State* lua, Function function, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

template <class Returner, class T, class R>
struct LuaInvoker<Returner, R (T::*)()>
{
    typedef R (T::* Function)();
    static int call( lua_State* lua, Function function, T* this_pointer );
};

template <class Returner, class T>
struct LuaInvoker<Returner, void (T::*)()>
{
    typedef void (T::* Function)();
    static int call( lua_State* lua, Function function, T* this_pointer );
};

template <class Returner, class T, class R>
struct LuaInvoker<Returner, R (T::*)() const>
{
    typedef R (T::* Function)() const;
    static int call( lua_State* lua, Function function, const T* this_pointer );
};

template <class Returner, class T>
struct LuaInvoker<Returner, void (T::*)() const>
{
    typedef void (T::* Function)() const;
    static int call( lua_State* lua, Function function, const T* this_pointer );
};

template <class Returner, class T, class R, class A0>
struct LuaInvoker<Returner, R (T::*)(A0)>
{
    typedef R (T::* Function)(A0);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0 );
};

template <class Returner, class T, class A0>
struct LuaInvoker<Returner, void (T::*)(A0)>
{
    typedef void (T::* Function)(A0);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0 );
};

template <class Returner, class T, class R, class A0>
struct LuaInvoker<Returner, R (T::*)(A0) const>
{
    typedef R (T::* Function)(A0) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, A0 a0 );
};

template <class Returner, class T, class A0>
struct LuaInvoker<Returner, void (T::*)(A0) const>
{
    typedef void (T::* Function)(A0) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, A0 a0 );
};

template <class Returner, class T, class R, class A0, class A1>
struct LuaInvoker<Returner, R (T::*)(A0, A1)>
{
    typedef R (T::* Function)(A0, A1);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class T, class A0, class A1>
struct LuaInvoker<Returner, void (T::*)(A0, A1)>
{
    typedef void (T::* Function)(A0, A1);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class T, class R, class A0, class A1>
struct LuaInvoker<Returner, R (T::*)(A0, A1) const>
{
    typedef R (T::* Function)(A0, A1) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class T, class A0, class A1>
struct LuaInvoker<Returner, void (T::*)(A0, A1) const>
{
    typedef void (T::* Function)(A0, A1) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1 );
};

template <class Returner, class T, class R, class A0, class A1, class A2>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2)>
{
    typedef R (T::* Function)(A0, A1, A2);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class T, class A0, class A1, class A2>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2)>
{
    typedef void (T::* Function)(A0, A1, A2);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class T, class R, class A0, class A1, class A2>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2) const>
{
    typedef R (T::* Function)(A0, A1, A2) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class T, class A0, class A1, class A2>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2) const>
{
    typedef void (T::* Function)(A0, A1, A2) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3)>
{
    typedef R (T::* Function)(A0, A1, A2, A3);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3)>
{
    typedef void (T::* Function)(A0, A1, A2, A3);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3) const>
{
    typedef void (T::* Function)(A0, A1, A2, A3) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4)>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4)>
{
    typedef void (T::* Function)(A0, A1, A2, A3, A4);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3, class A4>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4) const>
{
    typedef void (T::* Function)(A0, A1, A2, A3, A4) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4, A5);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4, A5)>
{
    typedef void (T::* Function)(A0, A1, A2, A3, A4, A5);
    static int call( lua_State* lua, Function function, T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

template <class Returner, class T, class R, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, R (T::*)(A0, A1, A2, A3, A4, A5) const>
{
    typedef R (T::* Function)(A0, A1, A2, A3, A4, A5) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

template <class Returner, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct LuaInvoker<Returner, void (T::*)(A0, A1, A2, A3, A4, A5) const>
{
    typedef void (T::* Function)(A0, A1, A2, A3, A4, A5) const;
    static int call( lua_State* lua, Function function, const T* this_pointer, typename traits::traits<A0>::parameter_type a0, typename traits::traits<A1>::parameter_type a1, typename traits::traits<A2>::parameter_type a2, typename traits::traits<A3>::parameter_type a3, typename traits::traits<A4>::parameter_type a4, typename traits::traits<A5>::parameter_type a5 );
};

}

}

#endif
