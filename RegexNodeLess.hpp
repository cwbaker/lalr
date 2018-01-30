#ifndef SWEET_LEXER_REGEXNODELESS_HPP_INCLUDED
#define SWEET_LEXER_REGEXNODELESS_HPP_INCLUDED

namespace sweet
{

namespace lalr
{

class RegexNode;

/**
// @internal
//
// Indirectly compare two RegexNodes.
*/
struct RegexNodeLess
{
    bool operator()( const RegexNode* lhs, const RegexNode* rhs ) const;
};

}

}

#endif
