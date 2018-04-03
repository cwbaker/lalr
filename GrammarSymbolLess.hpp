#ifndef LALR_GRAMMARSYMBOLLESS_HPP_INCLUDED
#define LALR_GRAMMARSYMBOLLESS_HPP_INCLUDED

namespace lalr
{

class GrammarSymbol;

/**
// Indirectly compare objects through two raw pointers.
*/
class GrammarSymbolLess
{
public:
    bool operator()( const GrammarSymbol* lhs, const GrammarSymbol* rhs ) const;
};

}

#endif
