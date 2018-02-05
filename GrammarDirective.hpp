#ifndef SWEET_PARSER_GRAMMARDIRECTIVE_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARDIRECTIVE_HPP_INCLUDED

#include "Associativity.hpp"
#include <vector>

namespace sweet
{

namespace lalr
{

class GrammarSymbol;

class GrammarDirective
{
    Associativity associativity_;
    std::vector<GrammarSymbol*> symbols_;

public:
    GrammarDirective( Associativity associativity );
    Associativity associativity() const;
    const std::vector<GrammarSymbol*>& symbols() const;
    void append_symbol( GrammarSymbol* symbol );
};

}

}

#endif
