//
// ptr_less.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_PTR_LESS_HPP_INCLUDED
#define SWEET_POINTER_PTR_LESS_HPP_INCLUDED

namespace sweet
{

namespace pointer
{

template <class Type> class ptr;

/**
// Compare objects through two ptrs.
*/
template <class Type>
class ptr_less
{
    public:
        bool operator()( const ptr<Type>& lhs, const ptr<Type>& rhs ) const
        {
            return *lhs < *rhs;
        }
};

}

using pointer::ptr_less;

}

#endif
