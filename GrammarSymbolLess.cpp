//
// GrammarSymbolLess.hpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarSymbolLess.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"

using namespace lalr;

bool GrammarSymbolLess::operator()( const GrammarSymbol* lhs, const GrammarSymbol* rhs ) const
{
    LALR_ASSERT( lhs->index() >= 0 );
    LALR_ASSERT( rhs->index() >= 0 );
    return lhs->index() < rhs->index();
}
