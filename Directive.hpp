#ifndef SWEET_LALR_DIRECTIVE_HPP_INCLUDED
#define SWEET_LALR_DIRECTIVE_HPP_INCLUDED

#include "Associativity.hpp"
#include <vector>

namespace sweet
{

namespace lalr
{

class Symbol;

class Directive
{
    Associativity associativity_;
    std::vector<Symbol*> symbols_;

public:
    Directive( Associativity associativity );
    Associativity associativity() const;
    const std::vector<Symbol*>& symbols() const;
    void append_symbol( Symbol* symbol );
};

}

}

#endif
