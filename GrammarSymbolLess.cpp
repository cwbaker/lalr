//
// GrammarSymbolLess.hpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarSymbolLess.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"

using namespace sweet::lalr;

bool GrammarSymbolLess::operator()( const GrammarSymbol* lhs, const GrammarSymbol* rhs ) const
{
    SWEET_ASSERT( lhs->index() >= 0 );
    SWEET_ASSERT( rhs->index() >= 0 );
    return lhs->index() < rhs->index();
}
