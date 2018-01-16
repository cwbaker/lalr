#ifndef SWEET_PARSER_GRAMMARDIRECTIVE_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARDIRECTIVE_HPP_INCLUDED

#include "SymbolAssociativity.hpp"
#include <vector>

namespace sweet
{

namespace parser
{

class GrammarSymbol;

class GrammarDirective
{
    SymbolAssociativity associativity_;
    std::vector<GrammarSymbol*> symbols_;

public:
    GrammarDirective( SymbolAssociativity associativity );
    SymbolAssociativity associativity() const;
    const std::vector<GrammarSymbol*>& symbols() const;
    void append_symbol( GrammarSymbol* symbol );
};

}

}

#endif
