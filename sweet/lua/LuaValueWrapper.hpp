//
// LuaValueWrapper.hpp
// Copyright (c) 2009 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUAVALUEWRAPPER_HPP_INCLUDED
#define SWEET_LUA_LUAVALUEWRAPPER_HPP_INCLUDED

namespace sweet
{

namespace lua
{

/**
// @internal
//
// A wrapper that captures the value of a type and indicates that it should
// be stored by value in the Lua virtual machine without undergoing any
// type conversion.
*/
template <class Type>
class LuaValueWrapper
{
    mutable Type value_;
    
    public:
        LuaValueWrapper( Type value )
        : value_( value )
        {
        }
        
        Type get_value() const
        {
            return value_;
        }
};

}

}

#endif
