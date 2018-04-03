#ifndef LALR_REGEXTOKENTYPE_HPP_INCLUDED
#define LALR_REGEXTOKENTYPE_HPP_INCLUDED

namespace lalr
{
    
/*
// The type of a token.
//
// The %RegexTokenType enumeration is specified in order of increasing priority 
// so that states that can match both a literal and a regular expression 
// result in the literal being matched.
//
// @relates RegexToken
*/
enum RegexTokenType
{
    TOKEN_NULL,
    TOKEN_REGULAR_EXPRESSION,
    TOKEN_LITERAL
};

}

#endif
