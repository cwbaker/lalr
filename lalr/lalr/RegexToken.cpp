//
// RegexToken.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexToken.hpp"
#include <algorithm>

using std::find;
using std::vector;
using namespace lalr;

RegexToken::RegexToken( RegexTokenType type, int line, int column, const void* symbol, const std::string& lexeme )
: type_( type ),
  line_( line ),
  column_( column ),
  symbol_( symbol ),
  lexeme_( lexeme ),
  conflicted_with_()
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

int RegexToken::column() const
{
    return column_;
}

const void* RegexToken::symbol() const
{
    return symbol_;
}

const std::string& RegexToken::lexeme() const
{
    return lexeme_;
}

bool RegexToken::conflicted_with( const RegexToken* token ) const
{
    return find( conflicted_with_.begin(), conflicted_with_.end(), token ) != conflicted_with_.end();
}

void RegexToken::add_conflicted_with( const RegexToken* token ) const
{
    if ( !conflicted_with(token) )
    {
        conflicted_with_.push_back( token );
    }
}
