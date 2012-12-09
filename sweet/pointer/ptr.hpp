//
// ptr.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_PTR_HPP_INCLUDED
#define SWEET_POINTER_PTR_HPP_INCLUDED

#include <sweet/build.hpp>
#include <sweet/assert/assert.hpp>
#include <sweet/traits/traits.hpp>
#include <sweet/atomic/atomic.hpp>
#include "declspec.hpp"
#include "ReferenceCount.hpp"
#include "ReferenceCountTemplate.hpp"
#include "enable_ptr_from_this.hpp"
#include "weak_ptr.hpp"

namespace sweet
{

namespace pointer
{

class ReferenceCount;
template <class Type> class enable_ptr_from_this;
struct static_cast_tag {};
struct const_cast_tag {};
struct dynamic_cast_tag {};

/**
// A reference counting smart pointer.
*/
template <class Type>
class ptr
{
    template <class OtherType> friend class ptr;
    template <class OtherType> friend class weak_ptr;

    Type* ptr_; ///< The object that this ptr points to.
    ReferenceCount* reference_count_; ///< The ReferenceCount that stores the reference counts for the object.

    public:
        ptr();
        ptr( const ptr& ptr );
        ptr& operator=( const ptr& ptr );
        template <class OtherType> explicit ptr( OtherType* ptr );
        template <class OtherType, class Deleter> ptr( OtherType* ptr, Deleter deleter );
        template <class OtherType> ptr( const ptr<OtherType>& ptr );
        template <class OtherType> ptr( const ptr<OtherType>& ptr, static_cast_tag );
        template <class OtherType> ptr( const ptr<OtherType>& ptr, const_cast_tag );
        template <class OtherType> ptr( const ptr<OtherType>& ptr, dynamic_cast_tag );
        template <class OtherType> ptr( const weak_ptr<OtherType>& weak_ptr );
        ~ptr();

        Type* operator->() const;
        Type& operator*() const;
        Type* get() const;

        bool operator==( const ptr& ptr ) const;
        bool operator!=( const ptr& ptr ) const;
        bool operator<( const ptr& ptr ) const;

        typedef Type* ptr<Type>::* unspecified_bool_type;
        operator unspecified_bool_type() const;

        void swap( ptr& ptr );
        void reset( Type* pptr = 0 );
        template <class Deleter> void reset( Type* pptr, Deleter deleter );
};

template <class Type, class OtherType> void initialize_enable_ptr_from_this( ReferenceCount* reference_count, const enable_ptr_from_this<Type>* enable_ptr_from_this, const OtherType* ptr );
void initialize_enable_ptr_from_this( ReferenceCount* reference_count, ... );
template <class Type, class OtherType> ptr<Type> static_pointer_cast( const ptr<OtherType>& ptr );
template <class Type, class OtherType> ptr<Type> const_pointer_cast( const ptr<OtherType>& ptr );
template <class Type, class OtherType> ptr<Type> dynamic_pointer_cast( const ptr<OtherType>& ptr );

}

using pointer::ptr;
using pointer::static_pointer_cast;
using pointer::const_pointer_cast;
using pointer::dynamic_pointer_cast;

}

#include "ptr_traits.ipp"
#include "ptr.ipp"
#include "ReferenceCountTemplate.ipp"

#endif
