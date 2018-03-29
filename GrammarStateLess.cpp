//
// GrammarStateLess.hpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarStateLess.hpp"
#include "GrammarState.hpp"

using namespace sweet::lalr;

bool GrammarStateLess::operator()( const std::shared_ptr<GrammarState>& lhs, const std::shared_ptr<GrammarState>& rhs ) const
{
    return *lhs < *rhs;
}
