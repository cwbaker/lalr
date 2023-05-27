//
// GrammarProductionLess.cpp
// Copyright (c) Charles Baker. All rights reserved.
//
#pragma once

#include "GrammarProductionLess.hpp"
#include "GrammarProduction.hpp"
#include "assert.hpp"

namespace lalr
{

bool GrammarProductionLess::operator()( const GrammarProduction* lhs, const GrammarProduction* rhs ) const
{
    LALR_ASSERT( lhs->index() >= 0 );
    LALR_ASSERT( rhs->index() >= 0 );
    return lhs->index() < rhs->index();
}

}
