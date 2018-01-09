//
// LexerToken.hpp
// Copyright (c) 2010 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_TOKEN_HPP_INCLUDED
#define SWEET_LEXER_TOKEN_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

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

/**
// A token to be recognized by a lexical analyzer.
*/
class SWEET_LEXER_DECLSPEC LexerToken
{
    LexerTokenType type_; ///< The type of this LexerToken (literal or regular expression).
    int line_; ///< The line to use when resolving token conflicts and reporting errors.
    const void* symbol_; ///< The symbol to return when this token is matched in input.
    std::string lexeme_; ///< The literal or regular expression pattern to match for this token.
    
    public:
        LexerToken( LexerTokenType type, int line, const void* symbol, const std::string& lexeme );
        LexerTokenType type() const;
        int line() const;
        const void* symbol() const;
        const std::string& lexeme() const;
};

}

}

#endif