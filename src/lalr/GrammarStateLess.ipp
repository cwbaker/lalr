//
// GrammarStateLess.ipp
// Copyright (c) Charles Baker. All rights reserved.
//
#pragma once

#include "GrammarStateLess.hpp"
#include "GrammarState.hpp"

namespace lalr
{

bool GrammarStateLess::operator()( const std::shared_ptr<GrammarState>& lhs, const std::shared_ptr<GrammarState>& rhs ) const
{
    return *lhs < *rhs;
}

}
