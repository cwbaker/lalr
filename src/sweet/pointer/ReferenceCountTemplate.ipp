//
// ReferenceCountTemplate.ipp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_REFERENCECOUNTTEMPLATE_IPP_INCLUDED
#define SWEET_POINTER_REFERENCECOUNTTEMPLATE_IPP_INCLUDED

#include "ReferenceCountTemplate.hpp"

namespace sweet
{

namespace pointer
{

/**
// Constructor.
//
// @param object
//  The object to count references for.
//
// @param deleter
//  The functor to use to delete \e object when its reference count drops to
//  0.
*/
template <class Type, class Deleter> ReferenceCountTemplate<Type, Deleter>::ReferenceCountTemplate( Type* object, Deleter deleter )
: ReferenceCount(),
  object_( object ),
  deleter_( deleter )
{
}

/**
// Destroy the object that this ReferenceCountTemplate is counting references
// to.
*/
template <class Type, class Deleter> void ReferenceCountTemplate<Type, Deleter>::dispose()
{
    SWEET_ASSERT( object_ );
    deleter_( object_ );
    object_ = 0;
}

/**
// Destroy this ReferenceCountTemplate object.
*/
template <class Type, class Deleter> void ReferenceCountTemplate<Type, Deleter>::destroy()
{
    delete this;
}

/**
// Default deleter for reference counted pointers.
//
// @param object
//  The object to delete.
//
// @relates ReferenceCountTemplate.
*/
template <class Type> void default_deleter( Type* object )
{
    delete object;
}

}

}

#endif
