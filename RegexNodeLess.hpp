#ifndef LALR_REGEXNODELESS_HPP_INCLUDED
#define LALR_REGEXNODELESS_HPP_INCLUDED

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

#endif
