//
// GrammarDirective.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarDirective.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

GrammarDirective::GrammarDirective( SymbolAssociativity associativity )
: associativity_( associativity ),
  symbols_()
{
}

SymbolAssociativity GrammarDirective::associativity() const
{
    return associativity_;
}

const std::vector<GrammarSymbol*>& GrammarDirective::symbols() const
{
    return symbols_;
}

void GrammarDirective::append_symbol( GrammarSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    symbols_.push_back( symbol );
}
