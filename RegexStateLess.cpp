//
// RegexStateLess.cpp
// Copyright (c) Charles Baker. All rights reserved
//

#include "RegexStateLess.hpp"
#include "RegexState.hpp"

using namespace lalr;

bool RegexStateLess::operator()( const std::unique_ptr<RegexState>& lhs, const std::unique_ptr<RegexState>& rhs ) const
{
    return *lhs < *rhs;
}
