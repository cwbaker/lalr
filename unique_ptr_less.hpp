#ifndef SWEET_LALR_UNIQUE_PTR_LESS_HPP_INCLUDED
#define SWEET_LALR_UNIQUE_PTR_LESS_HPP_INCLUDED

#include <memory>

namespace sweet
{

namespace lalr
{

/**
// Indirectly compare objects through two `std::unique_ptr<>` objects.
*/
template <class Type>
class unique_ptr_less
{
public:
    bool operator()( const std::unique_ptr<Type>& lhs, const std::unique_ptr<Type>& rhs ) const
    {
        return *lhs < *rhs;
    }
};

}

using lalr::unique_ptr_less;

}

#endif
