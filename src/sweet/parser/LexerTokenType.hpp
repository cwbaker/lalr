#ifndef SWEET_LEXER_LEXERTOKENTYPE_HPP_INCLUDED
#define SWEET_LEXER_LEXERTOKENTYPE_HPP_INCLUDED

namespace sweet
{

namespace lexer
{
    
/*
// The type of a token.
//
// The %LexerTokenType enumeration is specified in order of increasing priority 
// so that states that can match both a literal and a regular expression 
// result in the literal being matched.
//
// @relates LexerToken
*/
enum LexerTokenType
{
    TOKEN_NULL,
    TOKEN_REGULAR_EXPRESSION,
    TOKEN_LITERAL
};

}

}

#endif
