//
// ReferenceCountTemplate.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_REFERENCECOUNTTEMPLATE_HPP_INCLUDED
#define SWEET_POINTER_REFERENCECOUNTTEMPLATE_HPP_INCLUDED

namespace sweet
{

namespace pointer
{

/**
// The implementation of a ReferenceCount for a particular type.
*/
template <class Type, class Deleter>
class ReferenceCountTemplate : public ReferenceCount
{
    Type* object_; ///< The object that is pointed to.
    Deleter deleter_; ///< The mechanism that is used to delete the object.

    public:
        ReferenceCountTemplate( Type* object, Deleter deleter );
        void dispose();
        void destroy();        
};

template <class Type> void default_deleter( Type* object );

}

}

#endif
