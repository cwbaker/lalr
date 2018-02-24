#ifndef SWEET_LALR_GRAMMARDIRECTIVE_HPP_INCLUDED
#define SWEET_LALR_GRAMMARDIRECTIVE_HPP_INCLUDED

#include "Associativity.hpp"
#include <vector>

namespace sweet
{

namespace lalr
{

class LalrSymbol;

class GrammarDirective
{
    Associativity associativity_;
    std::vector<LalrSymbol*> symbols_;

public:
    GrammarDirective( Associativity associativity );
    Associativity associativity() const;
    const std::vector<LalrSymbol*>& symbols() const;
    void append_symbol( LalrSymbol* symbol );
};

}

}

#endif
