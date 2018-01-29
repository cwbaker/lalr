//
// LexerToken.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerToken.hpp"

using namespace sweet::lalr;

LexerToken::LexerToken( LexerTokenType type, int line, const void* symbol, const std::string& lexeme )
: type_( type ),
  line_( line ),
  symbol_( symbol ),
  lexeme_( lexeme )
{
}

LexerTokenType LexerToken::type() const
{
    return type_;
}

int LexerToken::line() const
{
    return line_;
}

const void* LexerToken::symbol() const
{
    return symbol_;
}

const std::string& LexerToken::lexeme() const
{
    return lexeme_;
}
