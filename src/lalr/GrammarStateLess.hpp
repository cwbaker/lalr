#pragma once

#include "GrammarState.hpp"
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
    inline bool operator()( const std::shared_ptr<GrammarState>& lhs, const std::shared_ptr<GrammarState>& rhs ) const;
};

}

#include "GrammarStateLess.ipp"
