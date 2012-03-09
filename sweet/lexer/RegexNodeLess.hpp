//
// RegexNodeLess.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_REGEXNODELESS_HPP_INCLUDED
#define SWEET_LEXER_REGEXNODELESS_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace lexer
{

class RegexNode;

/**
// @internal
//
// Indirectly compare two RegexNodes.
*/
struct SWEET_LEXER_DECLSPEC RegexNodeLess
{
    bool operator()( const RegexNode* lhs, const RegexNode* rhs ) const;
};

}

}

#endif
