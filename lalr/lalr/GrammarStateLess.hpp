#ifndef LALR_GRAMMARSTATELESS_HPP_INCLUDED
#define LALR_GRAMMARSTATELESS_HPP_INCLUDED

#include <memory>

namespace lalr
{

class GrammarState;

/**
// Indirectly compare objects through two raw pointers.
*/
class GrammarStateLess
{
public:
    bool operator()( const std::shared_ptr<GrammarState>& lhs, const std::shared_ptr<GrammarState>& rhs ) const;
};

}

#endif
