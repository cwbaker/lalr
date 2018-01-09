//
// enable_ptr_from_this.hpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_ENABLE_PTR_FROM_THIS_HPP_INCLUDED
#define SWEET_POINTER_ENABLE_PTR_FROM_THIS_HPP_INCLUDED

#include <sweet/build.hpp>
#include <sweet/assert/assert.hpp>
#include "ptr.hpp"
#include "weak_ptr.hpp"

namespace sweet
{

namespace pointer
{

/**
// A base class to allow construction of a ptr<> from a raw pointer.
*/
template <class Type>
class enable_ptr_from_this
{
    protected:
        enable_ptr_from_this()
        : weak_this_()
        {
        }
        
        enable_ptr_from_this( const enable_ptr_from_this& )
        : weak_this_()
        {
        }
        
        enable_ptr_from_this& operator=( const enable_ptr_from_this& )
        {
            return *this;
        }
        
        ~enable_ptr_from_this()
        {
        }
    
    public:
        ptr<Type> ptr_from_this()
        {
            ptr<Type> ptr( weak_this_ );
            SWEET_ASSERT( ptr.get() == this );
            return ptr;
        }

        ptr<const Type> ptr_from_this() const
        {
            ptr<const Type> ptr( weak_this_ );
            SWEET_ASSERT( ptr.get() == this );
            return ptr;
        }

        mutable weak_ptr<Type> weak_this_;
};

}

using pointer::enable_ptr_from_this;

}

#endif