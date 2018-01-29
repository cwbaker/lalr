//
// RegexNodeLess.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexNodeLess.hpp"
#include "RegexNode.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet;
using namespace sweet::parser;

/**
// Compare two RegexNodes.
//
// @param lhs
//  The first RegexNode to compare.
//
// @param rhs
//  The second RegexNode to compare.
//
// @return
//  True if the index of \e lhs is less than the index of \e rhs otherwise 
//  false.
*/
bool RegexNodeLess::operator()( const RegexNode* lhs, const RegexNode* rhs ) const
{
    SWEET_ASSERT( lhs );
    SWEET_ASSERT( rhs );
    return *lhs < *rhs;
}
