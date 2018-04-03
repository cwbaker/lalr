//
// RegexToken.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexToken.hpp"

using namespace lalr;

RegexToken::RegexToken( RegexTokenType type, int line, const void* symbol, const std::string& lexeme )
: type_( type ),
  line_( line ),
  symbol_( symbol ),
  lexeme_( lexeme )
{
}

RegexTokenType RegexToken::type() const
{
    return type_;
}

int RegexToken::line() const
{
    return line_;
}

const void* RegexToken::symbol() const
{
    return symbol_;
}

const std::string& RegexToken::lexeme() const
{
    return lexeme_;
}
