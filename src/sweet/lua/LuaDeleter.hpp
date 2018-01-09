//
// LuaDeleter.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_LUADELETER_HPP_INCLUDED
#define SWEET_LUA_LUADELETER_HPP_INCLUDED

namespace sweet
{

namespace lua
{

template <class Type, class Binder> class LuaDeleter
{
    Binder* binder_;

public:
    LuaDeleter( Binder* binder )
    : binder_( binder )
    {
        SWEET_ASSERT( binder_ );
    }
    
    void operator()( Type* object ) const
    {
        SWEET_ASSERT( object );
        binder_->destroy( object );
        delete object;
        object = NULL;
    }
};

template <class Type, class Binder> LuaDeleter<Type, Binder> deleter( Binder* binder )
{
    return LuaDeleter<Type, Binder>( binder );
}

}

}

#endif
