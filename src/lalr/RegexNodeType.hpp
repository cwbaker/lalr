#ifndef LALR_REGEXNODETYPE_HPP_INCLUDED
#define LALR_REGEXNODETYPE_HPP_INCLUDED

namespace lalr
{

/**
// @internal
//
// The type of a node in a parsed regular expression.
//
// @relates RegexNode
*/
enum RegexNodeType
{
    LEXER_NODE_NULL,
    LEXER_NODE_CAT,
    LEXER_NODE_OR,
    LEXER_NODE_STAR,
    LEXER_NODE_PLUS,
    LEXER_NODE_OPTIONAL,
    LEXER_NODE_SYMBOL,
    LEXER_NODE_ACTION,
    LEXER_NODE_COUNT
};

}

#endif
