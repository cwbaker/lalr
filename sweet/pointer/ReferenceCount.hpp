//
// ReferenceCount.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_REFERENCECOUNT_HPP_INCLUDED
#define SWEET_POINTER_REFERENCECOUNT_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace pointer
{

/**
// A count of references and weak references to an object.
*/
class SWEET_POINTER_DECLSPEC ReferenceCount
{
    int references_;
    int weak_references_;

    public:
        ReferenceCount();
        virtual ~ReferenceCount();

        void reference();
        bool promote_reference();
        void release();
        void weak_reference();
        void weak_release();

        virtual void dispose() = 0;
        virtual void destroy() = 0;
};

}

}

#endif
