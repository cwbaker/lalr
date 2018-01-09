//
// weak_ptr.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_WEAK_PTR_HPP_INCLUDED
#define SWEET_POINTER_WEAK_PTR_HPP_INCLUDED

#include <sweet/build.hpp>
#include <sweet/assert/assert.hpp>
#include <sweet/atomic/atomic.hpp>
#include "declspec.hpp"
#include "ReferenceCount.hpp"
#include "ReferenceCountTemplate.hpp"

namespace sweet
{

namespace pointer
{

template <class Type> class ptr;

/**
// A weak reference counted smart pointer.
*/
template <class Type>
class weak_ptr
{
    template <class OtherType> friend class ptr;

    Type* ptr_; ///< The object that this weak_ptr points to.
    ReferenceCount* reference_count_; ///< The ReferenceCount that stores the reference counts for the object.

    public:
        weak_ptr();
        weak_ptr( const weak_ptr& weak_ptr );
        weak_ptr& operator=( const weak_ptr& weak_ptr );
        weak_ptr( const ptr<Type>& ptr );
        weak_ptr& operator=( const ptr<Type>& ptr );
        ~weak_ptr();

        ptr<Type> lock() const;

        typedef Type* weak_ptr<Type>::* unspecified_bool_type;
        operator unspecified_bool_type() const;
        void assign( Type* ptr, ReferenceCount* reference_count );

        void swap( weak_ptr& weak_ptr );
        void reset();
};

}

using pointer::weak_ptr;

}

#include "weak_ptr.ipp"
#include "ReferenceCountTemplate.ipp"

#endif
