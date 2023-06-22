//
// GrammarParser.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarParser.hpp"
#include "Grammar.hpp"
#include "Parser.ipp"
#include "ParserStateMachine.hpp"
#include "ErrorPolicy.hpp"
#include "assert.hpp"
#include <string>

using std::string;
using namespace lalr;

GrammarParser::GrammarParser()
: error_policy_( nullptr )
, grammar_( nullptr )
, position_( nullptr )
, end_( nullptr )
, line_position_( nullptr )
, line_( 1 )
, lexeme_()
, errors_( 0 )
{
    const size_t lexeme_reserve = 256;
    lexeme_.reserve( lexeme_reserve );
}

int GrammarParser::parse( const char* start, const char* finish, ErrorPolicy* error_policy, Grammar* grammar )
{
    LALR_ASSERT( start );
    LALR_ASSERT( finish );
    LALR_ASSERT( grammar );
    error_policy_ = error_policy;
    grammar_ = grammar;
    line_position_ = position_ = start;
    end_ = finish;
    line_ = 1;
    errors_ = 0;
    if ( !match_grammar() )
    {
        error( LALR_ERROR_SYNTAX, "parsing grammar failed" );
    }
    return errors_;
}

bool GrammarParser::match_grammar()
{
    if ( match_identifier() )
    {
        grammar_->grammar( lexeme_.c_str() );
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
            grammar_->regex( lexeme_.c_str(), line_, column() );
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
        grammar_->production( lexeme_.c_str(), line_, column() );
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
        grammar_->error( line_, column() );
        return true;
    }
    else if ( match_literal() )
    {
        grammar_->literal( lexeme_.c_str(), line_, column() );
        return true;
    }
    else if ( match_regex() )
    {
        grammar_->regex( lexeme_.c_str(), line_, column() );
        return true;
    }
    else if ( match_identifier() )
    {
        grammar_->identifier( lexeme_.c_str(), line_, column() );
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
    else if ( match("%none") || match("%nonassoc"))
    {
        grammar_->none( line_ );
        return true;
    }
    else if ( match("%precedence") )
    {
        grammar_->assoc_prec( line_ );
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
    if ( match("%precedence") || match("%prec") )
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
            grammar_->action( lexeme_.c_str(), line_, column() );
        }
        expect( "]" );
        return true;
    }
    grammar_->end_expression( line_, column() );
    return false;
}

bool GrammarParser::match_error()
{
    return match( "error" );
}

bool GrammarParser::match_literal()
{
    match_whitespace_and_comments();
    if ( match("'") )
    {
        bool escaped = false;
        const char* position = position_;
        while ( position != end_ && (*position != '\'' || escaped) && !is_new_line(position) )
        {
            escaped = *position == '\\';
            ++position;
            if ( *position == '\\' && escaped )
            {
                ++position;
                escaped = false;
            }
        }

        if ( position == end_ || !is_new_line(position) )
        {
            lexeme_.assign( position_, position );
            position_ = position;
            expect( "'" );
            if ( lexeme_.empty() )
            {
                error( LALR_ERROR_EMPTY_LITERAL, "empty literal" );
            }
            return true;
        }

        error( LALR_ERROR_UNTERMINATED_LITERAL, "unterminated literal" );
        position_ = position;
        return false;
    }
    return false;
}

bool GrammarParser::match_regex()
{
    match_whitespace_and_comments();
    if ( match("\"") )
    {
        bool escaped = false;
        const char* position = position_;
        while ( position != end_ && (*position != '"' || escaped) && !is_new_line(position) )
        {
            escaped = *position == '\\';
            ++position;
            if ( *position == '\\' && escaped )
            {
                ++position;
                escaped = false;
            }
        }

        if ( position == end_ || !is_new_line(position) )
        {
            lexeme_.assign( position_, position );
            position_ = position;
            expect( "\"" );
            if ( lexeme_.empty() )
            {
                error( LALR_ERROR_EMPTY_LITERAL, "empty regex" );
            }
            return true;
        }

        error( LALR_ERROR_UNTERMINATED_LITERAL, "unterminated regex" );
        position_ = position;
        return false;
    }
    return false;
}

bool GrammarParser::match_identifier()
{
    match_whitespace_and_comments();
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

bool GrammarParser::match_whitespace_and_comments()
{
    while ( match_whitespace() || match_line_comment() || match_block_comment() )
    {
    }
    return true;
}

bool GrammarParser::match_whitespace()
{
    const char* position = position_;
    if ( position != end_ && isspace(*position) )
    {
        while ( position != end_ && isspace(*position) )
        {
            if ( is_new_line(position) )
            {
                ++line_;
                line_position_ = position;
            }
            ++position;
        }
        position_ = position;
        return true;
    }
    return false;
}

bool GrammarParser::match_line_comment()
{
    if ( match_without_skipping_whitespace("//") )
    {
        const char* position = position_;
        while ( position != end_ && !is_new_line(position) )
        {
            ++position;
        }
        position_ = new_line( position );
        return true;
    }
    return false;
}

bool GrammarParser::match_block_comment()
{
    if ( match_without_skipping_whitespace("/*") )
    {
        bool done = false;
        const char* position = position_;
        while ( position != end_ && !done )
        {
            if ( *position == '*' )
            {
                ++position;
                if ( position != end_ && *position == '/' )
                {
                    done = true;
                    ++position;
                }
            }
            else if ( is_new_line(position) )
            {
                position = new_line( position );
            }
            else
            {
                ++position;
            }
        }
        position_ = position;
        return true;
    }
    return false;
}

bool GrammarParser::match_end()
{
    match_whitespace_and_comments();
    return position_ == end_;
}

bool GrammarParser::match( const char* keyword )
{
    match_whitespace_and_comments();
    return match_without_skipping_whitespace( keyword );
}

bool GrammarParser::match_without_skipping_whitespace( const char* lexeme )
{
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
    error( LALR_ERROR_SYNTAX, "expected '%s' not found", lexeme );
    return false;
}

void GrammarParser::error( int error, const char* format, ... )
{
    LALR_ASSERT( format );
    ++errors_;
    if ( error_policy_ )
    {
        va_list args;
        va_start( args, format );
        error_policy_->lalr_error( line_, column(), error, format, args );
        va_end( args );
    }
}

const char* GrammarParser::new_line( const char* position )
{
    if ( position != end_ )
    {
        if ( *position == '\n' )
        {
            ++position;
            if ( position != end_ && *position == '\r' )
            {
                ++position;
            }
            ++line_;
            line_position_ = position;
        }
        else if ( *position == '\r' )
        {
            ++position;
            if ( position != end_ && *position == '\n' )
            {
                ++position;
            }
            ++line_;
            line_position_ = position;
        }
    }
    return position;
}

bool GrammarParser::is_new_line( const char* position )
{
    return *position == '\n' || *position == '\r';
}

int GrammarParser::column()
{
    LALR_ASSERT( position_ );
    LALR_ASSERT( position_ >= line_position_ );
    return int(position_ - line_position_);
}
