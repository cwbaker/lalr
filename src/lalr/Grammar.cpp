//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "GrammarParser.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "GrammarAction.hpp"
#include "GrammarGenerator.hpp"
#include "GrammarCompiler.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "RegexCompiler.hpp"
#include "RegexTokenType.hpp"
#include "assert.hpp"

using std::string;
using std::vector;
using std::set;
using std::unique_ptr;
using namespace lalr;

Grammar::Grammar()
: identifier_()
, symbols_()
, productions_()
, actions_()
, whitespace_tokens_()
, active_whitespace_directive_( false )
, active_precedence_directive_( false )
, associativity_( ASSOCIATE_NULL )
, precedence_( 0 )
, active_production_( nullptr )
, active_symbol_( nullptr )
, start_symbol_( nullptr )
, end_symbol_( nullptr )
, error_symbol_( nullptr )
, whitespace_symbol_( nullptr )
{
    start_symbol_ = add_symbol( ".start", 0, LEXEME_NULL, SYMBOL_NON_TERMINAL );
    end_symbol_ = add_symbol( ".end", 0, LEXEME_NULL, SYMBOL_END );
    error_symbol_ = add_symbol( "error", 0, LEXEME_NULL, SYMBOL_NULL );
    whitespace_symbol_ = add_symbol( ".whitespace", 0, LEXEME_NULL, SYMBOL_NULL );
}

Grammar::~Grammar()
{
}

const std::string& Grammar::identifier() const
{
    return identifier_;
}

std::vector<std::unique_ptr<GrammarSymbol>>& Grammar::symbols()
{
    return symbols_;
}

std::vector<std::unique_ptr<GrammarProduction>>& Grammar::productions()
{
    return productions_;
}

std::vector<std::unique_ptr<GrammarAction>>& Grammar::actions()
{
    return actions_;
}

const std::vector<RegexToken>& Grammar::whitespace_tokens() const
{
    return whitespace_tokens_;
}

GrammarSymbol* Grammar::start_symbol() const
{
    return start_symbol_;
}

GrammarSymbol* Grammar::end_symbol() const
{
    return end_symbol_;
}

GrammarSymbol* Grammar::error_symbol() const
{
    return error_symbol_;
}

GrammarSymbol* Grammar::whitespace_symbol() const
{
    return whitespace_symbol_;
}

Grammar& Grammar::grammar( const std::string& identifier )
{
    identifier_ = identifier;
    return *this;
}

Grammar& Grammar::left( int /*line*/ )
{
    associativity_ = ASSOCIATE_LEFT;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::right( int /*line*/ )
{
    associativity_ = ASSOCIATE_RIGHT;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::none( int /*line*/ )
{
    associativity_ = ASSOCIATE_NONE;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::whitespace()
{
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = true;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::assoc_prec( int /*line*/ )
{
    associativity_ = ASSOCIATE_PREC;
    ++precedence_;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::precedence()
{
    LALR_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        active_precedence_directive_ = true;
    }
    return *this;
}

Grammar& Grammar::production( const char* identifier, int line )
{
    LALR_ASSERT( identifier );
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = non_terminal_symbol( identifier, line );
    return *this;
}

Grammar& Grammar::end_production()
{
    LALR_ASSERT( active_symbol_ );
    associativity_ = ASSOCIATE_NULL;
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::end_expression( int line )
{
    LALR_ASSERT( line >= 1 );
    // If there is an active symbol but no active production then an empty
    // production is being specified (the nil action marks the end of a 
    // production for which no symbols have been specified).
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }
    }
    active_production_ = nullptr;
    return *this;
}

Grammar& Grammar::error( int line )
{
    LALR_ASSERT( line >= 1 );
    if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = error_symbol();
        symbol->set_associativity( associativity_ );        
        symbol->set_precedence( precedence_ );        
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }
        active_production_->append_symbol( error_symbol() );            
    }
    return *this;
}

Grammar& Grammar::action( const char* identifier, int line )
{
    LALR_ASSERT( identifier );
    LALR_ASSERT( line >= 1 );
    LALR_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }
        active_production_->set_action( add_action(identifier) );
        active_production_ = nullptr;
    }
    return *this;
}

Grammar& Grammar::literal( const char* literal, int line )
{
    LALR_ASSERT( literal );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_whitespace_directive_ || associativity_ != ASSOCIATE_NULL || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( RegexToken(TOKEN_LITERAL, 0, 0, whitespace_symbol_, literal) );
    }
    else if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = literal_symbol( literal, line );
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }        
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = literal_symbol( literal, line );
            active_production_->set_precedence_symbol( symbol );
            symbol->set_referenced_in_precedence_directive( true );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( literal_symbol(literal, line) );
        }
    }
    return *this;
}

Grammar& Grammar::regex( const char* regex, int line )
{
    LALR_ASSERT( regex );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_whitespace_directive_ || associativity_ != ASSOCIATE_NULL || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( RegexToken(TOKEN_REGULAR_EXPRESSION, 0, 0, whitespace_symbol_, regex) );
    }
    else if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = regex_symbol( regex, line );
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = regex_symbol( regex, line );
            symbol->set_referenced_in_precedence_directive( true );
            active_production_->set_precedence_symbol( symbol );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( regex_symbol(regex, line) );
        }
    }
    return *this;
}

Grammar& Grammar::identifier( const char* identifier, int line )
{
    LALR_ASSERT( identifier );
    LALR_ASSERT( line >= 0 );
    LALR_ASSERT( active_symbol_ || associativity_ != ASSOCIATE_NULL );
    if ( associativity_ != ASSOCIATE_NULL )
    {
        GrammarSymbol* symbol = non_terminal_symbol( identifier, line );
        symbol->set_associativity( associativity_ );
        symbol->set_precedence( precedence_ );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = add_production( active_symbol_, line );
        }
        if ( active_precedence_directive_ )
        {
            GrammarSymbol* symbol = non_terminal_symbol( identifier, line );
            symbol->set_referenced_in_precedence_directive( true );
            active_production_->set_precedence_symbol( symbol );
            active_precedence_directive_ = false;
        }
        else
        {
            GrammarSymbol* symbol = non_terminal_symbol(identifier, line );
            symbol->set_referenced_in_rule(true);
            active_production_->append_symbol( symbol );
        }
    }
    return *this;
}

GrammarSymbol* Grammar::literal_symbol( const char* lexeme, int line )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol( lexeme, line, LEXEME_LITERAL, SYMBOL_TERMINAL );
}

GrammarSymbol* Grammar::regex_symbol( const char* lexeme, int line )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol( lexeme, line, LEXEME_REGULAR_EXPRESSION, SYMBOL_TERMINAL );
}

GrammarSymbol* Grammar::non_terminal_symbol( const char* lexeme, int line )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    return add_symbol( lexeme, line, LEXEME_NULL, SYMBOL_NON_TERMINAL );
}

GrammarSymbol* Grammar::add_symbol( const char* lexeme, int line, LexemeType lexeme_type, SymbolType symbol_type )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( line >= 0 );
    vector<unique_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && !(*i)->matches(lexeme, symbol_type) )
    {
        ++i;
    }    
    if ( i == symbols_.end() )
    {
        unique_ptr<GrammarSymbol> symbol( new GrammarSymbol(lexeme) );
        symbol->set_line( line );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        symbols_.push_back( std::move(symbol) );
        return symbols_.back().get();
    }

    GrammarSymbol* symbol = i->get();
    LALR_ASSERT( symbol );
    LALR_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

GrammarProduction* Grammar::add_production( GrammarSymbol* symbol, int line )
{
    LALR_ASSERT( symbol );
    LALR_ASSERT( line > 0 );
    if ( productions_.empty() )
    {
        LALR_ASSERT( start_symbol_ );
        unique_ptr<GrammarProduction> production( new GrammarProduction(int(productions_.size()), start_symbol_, 0, 0, NULL) );
        production->append_symbol( symbol );
        start_symbol_->append_production( production.get() );
        productions_.push_back( std::move(production) );
    }

    unique_ptr<GrammarProduction> production( new GrammarProduction(int(productions_.size()), symbol, line, -1, nullptr) );
    symbol->append_production( production.get() );
    productions_.push_back( std::move(production) );
    return productions_.back().get();
}

GrammarAction* Grammar::add_action( const char* identifier )
{
    LALR_ASSERT( identifier );
    vector<unique_ptr<GrammarAction>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        int index = int(actions_.size());
        unique_ptr<GrammarAction> action( new GrammarAction(index, identifier) );
        actions_.push_back( std::move(action) );
        return actions_.back().get();
    }
    return i->get();
}
