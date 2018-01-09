//
// LuaStackDeleter.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUASTACKDELETER_HPP_INCLUDED
#define SWEET_LUA_LUASTACKDELETER_HPP_INCLUDED

namespace sweet
{

namespace lua
{

template <class Type, class Binder> class LuaStackDeleter
{
    Binder* binder_;

public:
    LuaStackDeleter( Binder* binder )
    : binder_( binder )
    {
        SWEET_ASSERT( binder_ );
    }
    
    void operator()( Type* object ) const
    {
        SWEET_ASSERT( object );
        binder_->destroy( object );
    }
};

template <class Type, class Binder> LuaStackDeleter<Type, Binder> stack_deleter( Binder* binder )
{
    return LuaStackDeleter<Type, Binder>( binder );
}

}

}

#endif
