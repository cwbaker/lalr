#ifndef LALR_REGEXSTATELESS_HPP_INCLUDED
#define LALR_REGEXSTATELESS_HPP_INCLUDED

#include <memory>

namespace lalr
{

class RegexState;

/**
// Indirectly compare objects through two `std::unique_ptr<>` objects.
*/
class RegexStateLess
{
public:
    bool operator()( const std::unique_ptr<RegexState>& lhs, const std::unique_ptr<RegexState>& rhs ) const;
};

}

#endif
