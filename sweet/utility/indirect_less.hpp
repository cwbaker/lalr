//
// indirect_less.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_UTILITY_INDIRECT_LESS_HPP_INCLUDED
#define SWEET_UTILITY_INDIRECT_LESS_HPP_INCLUDED

namespace sweet
{

namespace utility
{

/**
// Indirectly compare objects through two pointers (or pointer like objects).
*/
template <class Type>
class indirect_less
{
    public:
        bool operator()( const Type* lhs, const Type* rhs ) const
        {
            return *lhs < *rhs;
        }
};

}

}

#endif
