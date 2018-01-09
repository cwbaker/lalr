//
// AddMember.ipp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDMEMBER_IPP_INCLUDED
#define SWEET_LUA_ADDMEMBER_IPP_INCLUDED

#include "AddMember.hpp"
#include "LuaConverter.ipp"
#include <sweet/assert/assert.hpp>

/**
// Set the metatable of the table that corresponds to a C++ object to the
// table that corresponds to \e object.
//
// This is commonly to implement simple prototype based object orientation 
// using a metatable with an "__index" metamethod that redirects unresolved
// index operations on an object to the object's prototype.
//
@code
class Foo
{
    int value_;

    public:
        Foo();
        void set_value( int value );
        int  get_value() const;
};

Foo foo;

Lua lua;
LuaObject foo_prototype( lua );
foo_prototype.members()
    .type( SWEET_STATIC_TYPEID(Foo) )
    ( "set_value", &Foo::set_value )
    ( "get_value", &Foo::get_value )
;

LuaObject foo_metatable( lua );
foo_metatable.members()
    ( "__index", &foo_prototype )
;

Foo foo;
lua.create( foo );
lua.members( foo )
    .type( SWEET_STATIC_TYPEID(Foo) )
    .this_pointer( &foo )
    .metatable( foo_metatable )
;
@endcode
//
// @param object
//  The object to set as the metatable for the C++ object.
//
// @return
//  This AddMember.
*/
template <class Type>
sweet::lua::AddMember& 
sweet::lua::AddMember::metatable( const Type& object )
{
    SWEET_ASSERT( add_member_helper_ );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<Type>::push( lua_state, object );
    lua_setmetatable( lua_state, -2 );

    return *this;
}


/**
// Set a variable to refer to an arbitrary value (including functions with
// no up values).
//
// @param name
//  The name to store the variable under.
//
// @param value
//  The value to store.
//
// @return
//  This AddMember object.
*/
template <class Type> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Type& value )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<Type>::push( lua_state, value );
    lua_setfield( lua_state, -2, name );    

    return *this;
}


/**
// Set a variable to refer to a function with one up value.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, 1, 2, 3, 4, 5, 6>::function, 2 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with two up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, 1, 2, 3, 4, 5>::function, 3 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with three up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1, class P2> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, 1, 2, 3, 4>::function, 4 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with four up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1, class P2, class P3> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, 1, 2, 3>::function, 5 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with five up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1, class P2, class P3, class P4> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, 1, 2>::function, 6 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with six up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1, class P2, class P3, class P4, class P5> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, LuaPosition<P5, 7>::position, 1>::function, 7 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a function with seven up values.
//
// @param name
//  The name to store the function under.
//
// @param function
//  The function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @param p6
//  The seventh up value.
//
// @return
//  This AddMember object.
*/
template <class Function, class P0, class P1, class P2, class P3, class P4, class P5, class P6> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = function;
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    LuaConverter<P6>::push( lua_state, p6 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, LuaPosition<P5, 7>::position, LuaPosition<P6, 8>::position>::function, 8 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with no up values.
//
// @param name
//  The name to store the variable under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, 1, 2, 3, 4, 5, 6, 7>::function, 1 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with one up value.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, 1, 2, 3, 4, 5, 6>::function, 2 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with two up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, 1, 2, 3, 4, 5>::function, 3 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with three up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1, class P2> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, 1, 2, 3, 4>::function, 4 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with four up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1, class P2, class P3> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, 1, 2, 3>::function, 5 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with five up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, 1, 2>::function, 6 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with six up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4, class P5> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, LuaPosition<P5, 7>::position, 1>::function, 7 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a yielding function with seven up values.
//
// @param name
//  The name to store the function under.
//
// @param policy_wrapper
//  The LuaPolicyWrapper that wraps the yielding function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @param p6
//  The seventh up value.
//
// @return
//  This AddMember object.
*/
template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4, class P5, class P6> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
    using traits::traits;

    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    void* copied_function = lua_newuserdata( lua_state, sizeof(Function) );
    *reinterpret_cast<Function*>(copied_function) = policy_wrapper.get_function();
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    LuaConverter<P6>::push( lua_state, p6 );
    lua_pushcclosure( lua_state, &LuaThunker<Function, LuaReturner<typename traits<Function>::return_type, POLICY>, LuaPosition<P0, 2>::position, LuaPosition<P1, 3>::position, LuaPosition<P2, 4>::position, LuaPosition<P3, 5>::position, LuaPosition<P4, 6>::position, LuaPosition<P5, 7>::position, LuaPosition<P6, 8>::position>::function, 8 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with one up value.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The up value.
//
// @return
//  This AddMember object.
*/
template <class P0> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 1 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with two up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 2 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with three up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1, class P2> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1, const P2& p2 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 3 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with four up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1, class P2, class P3> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 4 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with five up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1, class P2, class P3, class P4> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 5 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with six up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1, class P2, class P3, class P4, class P5> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 6 );
    lua_setfield( lua_state, -2, name );

    return *this;
}


/**
// Set a variable to refer to a raw function with seven up values.
//
// @param name
//  The name to store the function under.
//
// @param raw_wrapper
//  The LuaRawWrapper that wraps the raw function.
//
// @param p0
//  The first up value.
//
// @param p1
//  The second up value.
//
// @param p2
//  The third up value.
//
// @param p3
//  The fourth up value.
//
// @param p4
//  The fifth up value.
//
// @param p5
//  The sixth up value.
//
// @param p6
//  The seventh up value.
//
// @return
//  This AddMember object.
*/
template <class P0, class P1, class P2, class P3, class P4, class P5, class P6> 
sweet::lua::AddMember& 
sweet::lua::AddMember::operator()( const char* name, const LuaRawWrapper& raw_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 )
{
    SWEET_ASSERT( add_member_helper_ != NULL );
    SWEET_ASSERT( name != NULL );

    lua_State* lua_state = add_member_helper_->get_lua_state();
    LuaStackGuard guard( lua_state );
    LuaConverter<P0>::push( lua_state, p0 );
    LuaConverter<P1>::push( lua_state, p1 );
    LuaConverter<P2>::push( lua_state, p2 );
    LuaConverter<P3>::push( lua_state, p3 );
    LuaConverter<P4>::push( lua_state, p4 );
    LuaConverter<P5>::push( lua_state, p5 );
    LuaConverter<P6>::push( lua_state, p6 );
    lua_pushcclosure( lua_state, raw_wrapper.get_function(), 7 );
    lua_setfield( lua_state, -2, name );

    return *this;
}

#endif
