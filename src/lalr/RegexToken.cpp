//
// RegexToken.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexToken.hpp"
#include <algorithm>

using std::find;
using std::vector;
using namespace lalr;

RegexToken::RegexToken( RegexTokenType type, int line, int column, const void* symbol, const char* lexeme )
: type_( type )
, line_( line )
, column_( column )
, symbol_( symbol )
, lexeme_( lexeme )
, conflicted_with_()
{
}

RegexToken::RegexToken( const RegexToken& token, const void* symbol )
: type_( token.type_ )
, line_( token.line_ )
, column_( token.column_ )
, symbol_( symbol )
, lexeme_( token.lexeme_ )
, conflicted_with_()
{
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
