//
// AddParameter.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_ADDPARAMETER_HPP_INCLUDED
#define SWEET_LUA_ADDPARAMETER_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

namespace sweet
{

namespace lua
{

class AddParameterHelper;
class LuaNil;
class LuaGlobalEnvironment;
class LuaValue;
class LuaObject;
class LuaThread;
class Lua;

/**
// A helper that provides a convenient syntax for calling functions.
*/
class SWEET_LUA_DECLSPEC AddParameter
{
    AddParameterHelper* add_parameter_helper_; ///< The AddParameterHelper that stores the state for this AddParameter.
   
    public:
        AddParameter( AddParameterHelper* add_parameter_helper );

        AddParameter& operator()( const LuaNil& nil );
        AddParameter& operator()( const LuaGlobalEnvironment& global_environment );
        AddParameter& operator()( const LuaValue& value );
        AddParameter& operator()( bool value );
        AddParameter& operator()( int value );
        AddParameter& operator()( float value );
        AddParameter& operator()( const char* value );
        AddParameter& operator()( const std::string& value );
        AddParameter& copy_values_from_stack( int begin, int end );

        void end();
        void end( bool* return_value );
        void end( int* return_value );
        void end( float* return_value );
        void end( std::string* return_value );
        void end( void** return_value );
        template <class Type> void end( Type* return_value );
        
        template <typename Type> AddParameter& operator()( const Type& value );
};

}

}

#endif
