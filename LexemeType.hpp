#ifndef LALR_LEXEMETYPE_HPP_INCLUDED
#define LALR_LEXEMETYPE_HPP_INCLUDED

namespace lalr
{

enum LexemeType
{
    LEXEME_NULL, ///< Null lexeme type.
    LEXEME_LITERAL, ///< Literal.
    LEXEME_REGULAR_EXPRESSION, ///< Regular expression.
    LEXEME_TYPE_COUNT ///< The number of lexeme types.
};

}

#endif
