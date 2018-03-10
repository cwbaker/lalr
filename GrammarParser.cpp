//
// GrammarParser.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarParser.hpp"
#include "Grammar.hpp"
#include "Parser.ipp"
#include "ParserStateMachine.hpp"
#include "assert.hpp"
#include <string>

using std::string;
using namespace sweet::lalr;

GrammarParser::GrammarParser()
: grammar_( nullptr ),
  position_( nullptr ),
  end_( nullptr ),
  line_( 1 ),
  lexeme_(),
  successful_( false )
{
    const size_t lexeme_reserve = 256;
    lexeme_.reserve( lexeme_reserve );
}

bool GrammarParser::parse( const char* start, const char* finish, Grammar* grammar )
{
    SWEET_ASSERT( start );
    SWEET_ASSERT( finish );
    SWEET_ASSERT( grammar );
    grammar_ = grammar;
    position_ = start;
    end_ = finish;
    line_ = 1;
    successful_ = true;
    return match_grammar() && successful_;
}

bool GrammarParser::match_grammar()
{
    if ( match_identifier() )
    {
        expect( "{" );
        match_statements();
        expect( "}" );
        return match_end();
    }
    return false;
}

bool GrammarParser::match_statements()
{
    while ( match_statement() )
    {
    }
    return true; 
}

bool GrammarParser::match_statement()
{
    return 
        match_associativity_statement() ||
        match_whitespace_statement() ||
        match_production_statement()
    ;
}

bool GrammarParser::match_associativity_statement()
{
    if ( match_associativity() )
    {
        match_symbols();
        expect( ";" );
        return true;
    }
    return false;
}

bool GrammarParser::match_whitespace_statement()
{
    if ( match("%whitespace") )
    {
        grammar_->whitespace();
        if ( match_regex() )
        {
            grammar_->regex( lexeme_.c_str(), line_ );
        }
        expect( ";" );
        return true;
    }
    return false;
}

bool GrammarParser::match_production_statement()
{
    if ( match_identifier() )
    {
        grammar_->production( lexeme_.c_str(), line_ );
        expect( ":" );
        match_expressions();
        expect( ";" );
        grammar_->end_production();
        return true;
    }
    return false;
}

bool GrammarParser::match_symbols()
{
    while ( match_symbol() )
    {
    }
    return true;
}

bool GrammarParser::match_symbol()
{
    if ( match_error() )
    {
        grammar_->error();
        return true;
    }
    else if ( match_literal() )
    {
        grammar_->literal( lexeme_.c_str(), line_ );
        return true;
    }
    else if ( match_regex() )
    {
        grammar_->regex( lexeme_.c_str(), line_ );
        return true;
    }
    else if ( match_identifier() )
    {
        grammar_->identifier( lexeme_.c_str(), line_ );
        return true;
    }
    return false;
}

bool GrammarParser::match_associativity()
{
    if ( match("%left") )
    {
        grammar_->left( line_ );
        return true;
    }
    else if ( match("%right") )
    {
        grammar_->right( line_ );
        return true;
    }
    else if ( match("%none") )
    {
        grammar_->none( line_ );
        return true;
    }
    return false;
}

bool GrammarParser::match_expressions()
{
    match_expression();
    while ( match("|") && match_expression() )
    {
    }
    return true;
}

bool GrammarParser::match_expression()
{
    match_symbols();
    match_precedence();
    match_action();
    return true;
}

bool GrammarParser::match_precedence()
{
    if ( match("%precedence") )
    {
        grammar_->precedence();
        match_symbol();
        return true;
    }
    return false;
}

bool GrammarParser::match_action()
{
    if ( match("[") )
    {
        if ( match_identifier() )
        {
            grammar_->action( lexeme_.c_str() );
        }
        expect( "]" );
        return true;
    }
    grammar_->end_expression();
    return false;
}

bool GrammarParser::match_error()
{
    return match( "error" );
}

bool GrammarParser::match_literal()
{
    match_whitespace();
    if ( match("'") )
    {
        bool escaped = false;
        const char* position = position_;
        while ( position != end_ && (*position != '\'' || escaped) )
        {
            escaped = *position == '\\';
            ++position;
        }
        lexeme_.assign( position_, position );
        position_ = position;
        expect( "'" );
        return true;
    }
    return false;
}

bool GrammarParser::match_regex()
{
    match_whitespace();
    if ( match("\"") )
    {
        bool escaped = false;
        const char* position = position_;
        while ( position != end_ && (*position != '"' || escaped) )
        {
            escaped = *position == '\\';
            ++position;
        }
        lexeme_.assign( position_, position );
        position_ = position;
        expect( "\"" );
        return true;
    }
    return false;
}

bool GrammarParser::match_identifier()
{
    match_whitespace();
    const char* position = position_;
    if ( position != end_ && (isalnum(*position) || *position == '_') )
    {
        ++position;
        while ( position != end_ && (isalnum(*position) || isdigit(*position) || *position == '_') )
        {
            ++position;
        }
        lexeme_.assign( position_, position );
        position_ = position;
        return true;
    }
    return false;
}

bool GrammarParser::match_whitespace()
{
    bool newline = false;
    const char* position = position_;
    while ( position != end_ && isspace(*position) )
    {
        if ( !newline && (*position == '\n' || *position == '\r') )
        {
            newline = true;
            ++line_;
        }
        ++position;
        newline = false;
    }
    position_ = position;
    return true;
}

bool GrammarParser::match_end()
{
    match_whitespace();
    return position_ == end_;
}

bool GrammarParser::match( const char* lexeme )
{
    match_whitespace();
    const char* position = position_;
    while ( position != end_ && *lexeme != 0 && *position == *lexeme )
    {
        ++position;
        ++lexeme;
    }
    if ( *lexeme == 0 )
    {
        position_ = position;
        return true;
    }
    return false;
}

bool GrammarParser::expect( const char* lexeme )
{
    if ( match(lexeme) )
    {
        return true;
    }
    position_ = end_;
    successful_ = false;
    return false;
}
