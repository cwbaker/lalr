//
// ptr.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_PTR_IPP_INCLUDED
#define SWEET_POINTER_PTR_IPP_INCLUDED

namespace sweet
{

namespace pointer
{

template <class Type, class OtherType> 
void initialize_enable_ptr_from_this( ReferenceCount* reference_count, const enable_ptr_from_this<Type>* enable_ptr_from_this, const OtherType* ptr )
{
    SWEET_ASSERT( reference_count );
    SWEET_ASSERT( enable_ptr_from_this );
    SWEET_ASSERT( ptr );
    enable_ptr_from_this->weak_this_.assign( const_cast<OtherType*>(ptr), reference_count );
}

inline void initialize_enable_ptr_from_this( ReferenceCount* reference_count, ... )
{
}

template <class Type, class OtherType> ptr<Type> static_pointer_cast( const ptr<OtherType>& pptr )
{
    return ptr<Type>( pptr, static_cast_tag() );
}

template <class Type, class OtherType> ptr<Type> const_pointer_cast( const ptr<OtherType>& pptr )
{
    return ptr<Type>( pptr, const_cast_tag() );
}

template <class Type, class OtherType> ptr<Type> dynamic_pointer_cast( const ptr<OtherType>& pptr )
{
    return ptr<Type>( pptr, dynamic_cast_tag() );
}

/**
// Constructor.
*/
template <class Type>
ptr<Type>::ptr()
: ptr_( 0 ),
  reference_count_( 0 )
{
}

/**
// Copy constructor.
//
// @param ptr
//  The ptr to copy.
*/
template <class Type>
ptr<Type>::ptr( const ptr& ptr )
: ptr_( ptr.ptr_ ),
  reference_count_( ptr.reference_count_ )
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->reference();
    }
}

/**
// Assignment operator.
//
// @param ptr
//  The ptr to assign from.
//
// @return
//  This ptr.
*/
template <class Type>
ptr<Type>& 
ptr<Type>::operator=( const ptr& ptr )
{
    if ( this != &ptr && ptr_ != ptr.ptr_ )
    {
        if ( ptr_ )
        {
            SWEET_ASSERT( reference_count_ );
            reference_count_->release();
            ptr_ = 0;
            reference_count_ = 0;
        }

        if ( ptr.ptr_ )
        {
            SWEET_ASSERT( ptr.reference_count_ );
            ptr.reference_count_->reference();
            ptr_ = ptr.ptr_;
            reference_count_ = ptr.reference_count_;
        }
    }

    return *this;
}

/**
// Constructor.
//
// @param ptr
//  The object to refer to or null to not refer to any object.
*/
template <class Type>
template <class OtherType>
ptr<Type>::ptr( OtherType* ptr )
: ptr_( ptr ),
  reference_count_( 0 )
{
    if ( ptr_ )
    {
        reference_count_ = new ReferenceCountTemplate<Type, void (*)(Type*)>( ptr, &default_deleter<Type> );
        initialize_enable_ptr_from_this( reference_count_, ptr_, ptr );
    }
}

/**
// Constructor.
//
// Constuct a ptr that uses \e deleter to destroy \e ptr when its reference
// count drops to 0.
//
// @param ptr
//  The object to count references on.
//
// @param deleter
//  The functor to use to destroy \e ptr when its reference count drops to 0.
*/
template <class Type>
template <class OtherType, class Deleter>
ptr<Type>::ptr( OtherType* ptr, Deleter deleter )
: ptr_( ptr ),
  reference_count_( 0 )
{
    if ( ptr_ )
    {
        reference_count_ = new ReferenceCountTemplate<Type, Deleter>( ptr, deleter );
        initialize_enable_ptr_from_this( reference_count_, ptr, ptr );
    }
}

/**
// Constructor.
//
// @param ptr
//  The ptr to copy.
*/
template <class Type>
template <class OtherType> 
ptr<Type>::ptr( const ptr<OtherType>& ptr )
: ptr_( ptr.ptr_ ),
  reference_count_( ptr.reference_count_ )
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->reference();
    }
}

/**
// Constructor.
//
// Constructs a ptr<Type> from a ptr<OtherType> using a static cast to convert
// the address pointed to while sharing the reference count between the two
// ptrs.
//
// @param ptr
//  The pointer to \e OtherType to static cast to a pointer to \e Type and
//  store in this ptr.
*/
template <class Type>
template <class OtherType> 
ptr<Type>::ptr( const ptr<OtherType>& ptr, static_cast_tag )
: ptr_( static_cast<Type*>(ptr.ptr_) ),
  reference_count_( ptr.reference_count_ )
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->reference();
    }
}

/**
// Constructor.
//
// Constructs a ptr<Type> from a ptr<OtherType> using a const cast to convert
// the address pointed to while sharing the reference count between the two
// ptrs.
//
// @param ptr
//  The pointer to \e OtherType to const cast to a pointer to \e Type and
//  store in this ptr.
*/
template <class Type> 
template <class OtherType> 
ptr<Type>::ptr( const ptr<OtherType>& ptr, const_cast_tag )
: ptr_( const_cast<Type*>(ptr.ptr_) ),
  reference_count_( ptr.reference_count_ )
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->reference();
    }
}

/**
// Constructor.
//
// Constructs a ptr<Type> from a ptr<OtherType> using a dynamic cast to convert
// the address pointed to while sharing the reference count between the two
// ptrs.
//
// @param ptr
//  The pointer to \e OtherType to dynamic cast to a pointer to \e Type and
//  store in this ptr.
*/
template <class Type> 
template <class OtherType> 
ptr<Type>::ptr( const ptr<OtherType>& ptr, dynamic_cast_tag )
: ptr_( dynamic_cast<Type*>(ptr.ptr_) ),
  reference_count_( ptr.reference_count_ )
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->reference();
    }
}

/**
// Constructor.
//
// If the object that \e weak_ptr is pointing to has been destroyed then
// this ptr will be constructed as an empty ptr.  It is the client's 
// responsibility to check that a ptr is pointing at an object after it has
// been constructed from a weak_ptr.
//
// @param weak_ptr
//  The weak_ptr to construct from.
*/
template <class Type>
template <class OtherType>
ptr<Type>::ptr( const weak_ptr<OtherType>& weak_ptr )
: ptr_( weak_ptr.ptr_ ),
  reference_count_( weak_ptr.reference_count_ )
{
    if ( reference_count_ )
    {
        if ( !reference_count_->promote_reference() )
        {
            ptr_ = 0;
            reference_count_ = 0;
        }
    }
}

/**
// Destructor.
*/ 
template <class Type> 
ptr<Type>::~ptr()
{
    if ( ptr_ )
    {
        SWEET_ASSERT( reference_count_ );
        reference_count_->release();
        ptr_ = 0;
        reference_count_ = 0;
    }
}

/**
// Dereference operator.
//
// @return
//  The object that this ptr refers to.
*/
template <class Type>
Type* ptr<Type>::operator->() const
{
    SWEET_ASSERT( ptr_ );
    return ptr_;
}

/**
// Dereference operator.
//
// @return
//  The object that this ptr refers to.
*/
template <class Type>
Type& ptr<Type>::operator*() const
{
    SWEET_ASSERT( ptr_ );
    return *ptr_;
}

/**
// Get the object that this ptr refers to.
//
// @return
//  The object that this ptr refers to or null if this ptr doesn't refer to
//  any object.
*/
template <class Type>
Type* ptr<Type>::get() const
{
    return ptr_;
}

/**
// Equal operator.
//
// @param ptr
//  The ptr to compare this ptr with.
//
// @return
//  True if this ptr and \e ptr point to the same object otherwise false.
*/
template <class Type>
bool ptr<Type>::operator==( const ptr& ptr ) const
{
    return ptr_ == ptr.ptr_;
}

/**
// Not equal operator.
//
// @param ptr
//  The ptr to compare this ptr with.
//
// @return
//  True if this ptr and \e ptr point to different objects otherwise false.
*/
template <class Type>
bool ptr<Type>::operator!=( const ptr& ptr ) const
{
    return ptr_ != ptr.ptr_;
}

/**
// Less than operator.
//
// @param ptr
//  The ptr to compare this ptr with.
//
// @return
//  True if this ptr is less than \e ptr otherwise false.
*/
template <class Type>
bool ptr<Type>::operator<( const ptr& ptr ) const
{
    return ptr_ < ptr.ptr_;
}

/**
// Allow comparison as a pointer.
//
// @return
//  Non null if this ptr points to an object otherwise null.
*/
template <class Type> ptr<Type>::operator unspecified_bool_type() const
{
    return ptr_ ? &ptr<Type>::ptr_ : 0;
}

/**
// Swap this ptr with another.
//
// @param ptr
//  The ptr to swap with.
*/
template <class Type>
void ptr<Type>::swap( ptr& ptr )
{
    std::swap( ptr_, ptr.ptr_ );
    std::swap( reference_count_, ptr.reference_count_ );
}

/**
// Reset this ptr to refer to a different object.
//
// @param pptr
//  The object to refer to.
*/
template <class Type>
void ptr<Type>::reset( Type* pptr )
{
    SWEET_ASSERT( !pptr || pptr != ptr_ );
    ptr<Type>( pptr ).swap( *this );
}

/**
// Reset this ptr to refer to a different object that uses \e deleter to 
// destroy \e ptr when its reference count drops to 0.
//
// @param pptr
//  The object to refer to.
//
// @param deleter
//  The functor to use to destroy \e ptr when its reference count drops to 0.
*/
template <class Type> 
template <class Deleter> 
void ptr<Type>::reset( Type* pptr, Deleter deleter )
{
    SWEET_ASSERT( !pptr || pptr != ptr_ );
    ptr<Type>( pptr, deleter ).swap( *this );
}

}

}

#endif