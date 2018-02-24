//
// LalrDirective.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LalrDirective.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

LalrDirective::LalrDirective( Associativity associativity )
: associativity_( associativity ),
  symbols_()
{
}

Associativity LalrDirective::associativity() const
{
    return associativity_;
}

const std::vector<LalrSymbol*>& LalrDirective::symbols() const
{
    return symbols_;
}

void LalrDirective::append_symbol( LalrSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    symbols_.push_back( symbol );
}
