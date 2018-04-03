//
// RegexNodeLess.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexNodeLess.hpp"
#include "RegexNode.hpp"
#include "assert.hpp"

using namespace lalr;

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
    LALR_ASSERT( lhs );
    LALR_ASSERT( rhs );
    return *lhs < *rhs;
}
