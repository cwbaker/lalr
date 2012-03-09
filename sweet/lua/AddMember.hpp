//
// AddMember.hpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDMEMBER_HPP_INCLUDED
#define SWEET_LUA_ADDMEMBER_HPP_INCLUDED

#include "declspec.hpp"
#include "LuaNil.hpp"
#include "LuaGlobalEnvironment.hpp"
#include "AddGlobal.hpp"
#include "AddMember.hpp"
#include <sweet/rtti/Type.hpp>
#include <string>

namespace sweet
{

namespace lua
{

class AddMemberHelper;
class LuaObject;

/**
// A helper that provides a convenient syntax for adding member variables to
// objects.
*/
class SWEET_LUA_DECLSPEC AddMember
{
    AddMemberHelper* add_member_helper_;

    public:
        AddMember( AddMemberHelper* add_member_helper );
        ~AddMember();

        AddMember& this_pointer( void* value );
        AddMember& type( const rtti::Type& type );
        template <class Type> AddMember& metatable( const Type& object );

        AddMember& operator()( const char* name, const LuaNil& nil );
        AddMember& operator()( const char* name, const LuaGlobalEnvironment& global_environment );
        AddMember& operator()( const char* name, bool value );
        AddMember& operator()( const char* name, int value );
        AddMember& operator()( const char* name, float value );
        AddMember& operator()( const char* name, const char* value );
        AddMember& operator()( const char* name, const std::string& value );

        template <class Type> AddMember& operator()( const char* name, const Type& value );
        template <class Function, class P0> AddMember& operator()( const char* name, const Function& function, const P0& p0 );
        template <class Function, class P0, class P1> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1 );
        template <class Function, class P0, class P1, class P2> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2 );
        template <class Function, class P0, class P1, class P2, class P3> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3 );
        template <class Function, class P0, class P1, class P2, class P3, class P4> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 );
        template <class Function, class P0, class P1, class P2, class P3, class P4, class P5> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 );
        template <class Function, class P0, class P1, class P2, class P3, class P4, class P5, class P6> AddMember& operator()( const char* name, const Function& function, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 );

        template <class Function, int POLICY> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& value );
        template <class Function, int POLICY, class P0> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0 );
        template <class Function, int POLICY, class P0, class P1> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1 );
        template <class Function, int POLICY, class P0, class P1, class P2> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2 );
        template <class Function, int POLICY, class P0, class P1, class P2, class P3> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3 );
        template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 );
        template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4, class P5> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 );
        template <class Function, int POLICY, class P0, class P1, class P2, class P3, class P4, class P5, class P6> AddMember& operator()( const char* name, const LuaPolicyWrapper<Function, POLICY>& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 );

        AddMember& operator()( const char* name, const LuaRawWrapper& value );
        template <class P0> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0 );
        template <class P0, class P1> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1 );
        template <class P0, class P1, class P2> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1, const P2& p2 );
        template <class P0, class P1, class P2, class P3> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3 );
        template <class P0, class P1, class P2, class P3, class P4> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4 );
        template <class P0, class P1, class P2, class P3, class P4, class P5> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5 );
        template <class P0, class P1, class P2, class P3, class P4, class P5, class P6> AddMember& operator()( const char* name, const LuaRawWrapper& policy_wrapper, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6 );
};

}

}

#endif
