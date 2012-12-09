//
// weak_ptr.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_WEAK_PTR_IPP_INCLUDED
#define SWEET_POINTER_WEAK_PTR_IPP_INCLUDED

#include <algorithm>

namespace sweet
{

namespace pointer
{

/**
// Constructor.
*/
template <class Type> weak_ptr<Type>::weak_ptr()
: ptr_( 0 ),
  reference_count_( 0 )
{
}

/**
// Copy constructor.
//
// @param weak_ptr
//  The weak_ptr to copy.
*/
template <class Type> weak_ptr<Type>::weak_ptr( const weak_ptr& weak_ptr )
: ptr_( weak_ptr.ptr_ ),
  reference_count_( weak_ptr.reference_count_ )
{
    if ( reference_count_ )
    {
        reference_count_->weak_reference();
    }
}

/**
// Assignment operator.
//
// @param ptr
//  The weak_ptr to assign from.
//
// @return
//  This weak_ptr.
*/
template <class Type> weak_ptr<Type>& weak_ptr<Type>::operator=( const weak_ptr& weak_ptr )
{
    if ( this != &weak_ptr && reference_count_ != weak_ptr.reference_count_ )
    {
        if ( reference_count_ )
        {
            reference_count_->weak_release();
            ptr_ = 0;
            reference_count_ = 0;
        }

        if ( weak_ptr.reference_count_ )
        {
            weak_ptr.reference_count_->weak_reference();
            ptr_ = weak_ptr.ptr_;
            reference_count_ = weak_ptr.reference_count_;
        }
    }

    return *this;
}

/**
// Constructor.
//
// @param ptr
//  The ptr that refers to the object that this weak_ptr will refer to.
*/
template <class Type> weak_ptr<Type>::weak_ptr( const ptr<Type>& ptr )
: ptr_( ptr.ptr_ ),
  reference_count_( ptr.reference_count_ )
{
    if ( reference_count_ )
    {
        reference_count_->weak_reference();
    }
}

/**
// Assignment operator.
//
// @param ptr
//  The ptr that refers to the object that this weak_ptr will refer to.
//
// @return
//  This weak_ptr.
*/
template <class Type> weak_ptr<Type>& weak_ptr<Type>::operator=( const ptr<Type>& ptr )
{
    if ( reference_count_ != ptr.reference_count_ )
    {
        if ( reference_count_ )
        {
            reference_count_->weak_release();
            ptr_ = 0;
            reference_count_ = 0;
        }

        if ( ptr.reference_count_ )
        {
            ptr.reference_count_->weak_reference();
            ptr_ = ptr.ptr_;
            reference_count_ = ptr.reference_count_;
        }
    }

    return *this;
}

/**
// Destructor.
*/
template <class Type> weak_ptr<Type>::~weak_ptr()
{
    if ( reference_count_ )
    {
        reference_count_->weak_release();
        ptr_ = 0;
        reference_count_ = 0;
    }
}

/**
// Get a ptr to the object that this weak_ptr is referring to.
//
// @return
//  A ptr to the object or a ptr to null if the object has been destroyed.
*/
template <class Type> ptr<Type> weak_ptr<Type>::lock() const
{
    return ptr<Type>( *this );
}

/**
// Allow comparison as a pointer.
//
// @return
//  Non null if this weak_ptr refers to an object otherwise null.
*/
template <class Type> weak_ptr<Type>::operator unspecified_bool_type() const
{
    return reference_count_ ? &weak_ptr<Type>::ptr_ : 0;
}

/**
// Assign the object and ReferenceCount for this weak_ptr.
//
// @param ptr
//  The object that this weak_ptr is to refer to.
//
// @param reference_count
//  The ReferenceCount for this weak_ptr.
*/
template <class Type> void weak_ptr<Type>::assign( Type* ptr, ReferenceCount* reference_count )
{
    SWEET_ASSERT( !ptr_ );
    SWEET_ASSERT( !reference_count_ );

    if ( reference_count )
    {
        reference_count->weak_reference();
        ptr_ = ptr;
        reference_count_ = reference_count;
    }
}

/**
// Swap this weak_ptr with another.
//
// @param weak_ptr
//  The weak_ptr to swap with.
*/
template <class Type> void weak_ptr<Type>::swap( weak_ptr& weak_ptr )
{
    std::swap( reference_count_, weak_ptr.reference_count_ );
    std::swap( ptr_, weak_ptr.ptr_ );
}

/**
// Reset this weak_ptr to point to null.
*/
template <class Type> void weak_ptr<Type>::reset()
{
    if ( reference_count_ )
    {
        reference_count_->weak_release();
        ptr_ = 0;
        reference_count_ = 0;
    }
}

}

}

#endif
