//
// Directive.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Directive.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

Directive::Directive( Associativity associativity )
: associativity_( associativity ),
  symbols_()
{
}

Associativity Directive::associativity() const
{
    return associativity_;
}

const std::vector<Symbol*>& Directive::symbols() const
{
    return symbols_;
}

void Directive::append_symbol( Symbol* symbol )
{
    SWEET_ASSERT( symbol );
    symbols_.push_back( symbol );
}
