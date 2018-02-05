//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "GrammarDirective.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
#include "GrammarAction.hpp"
#include "LexerTokenType.hpp"
#include "assert.hpp"

using std::string;
using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

Grammar::Grammar()
: directives_(),
  symbols_(),
  productions_(),
  actions_(),
  whitespace_tokens_(),
  active_directive_( nullptr ),
  active_production_( nullptr ),
  active_symbol_( nullptr )
{
}

const std::vector<std::shared_ptr<GrammarDirective>>& Grammar::directives() const
{
    return directives_;
}

const std::vector<std::shared_ptr<GrammarSymbol>>& Grammar::symbols() const
{
    return symbols_;
}

const std::vector<std::shared_ptr<GrammarProduction>>& Grammar::productions() const
{
    return productions_;
}

const std::vector<LexerToken>& Grammar::whitespace_tokens() const
{
    return whitespace_tokens_;
}

Grammar& Grammar::begin()
{
    return *this;
}

Grammar& Grammar::left()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_LEFT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::right()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_RIGHT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::none()
{
    active_whitespace_directive_ = false;
    active_directive_ = directive( ASSOCIATE_NONE );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::whitespace()
{
    active_whitespace_directive_ = true;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::precedence( char literal )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_precedence_symbol( symbol(literal) );
    }
    return *this;
}

Grammar& Grammar::precedence( const char* regex )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_precedence_symbol( symbol(regex) );
    }
    return *this;
}

Grammar& Grammar::production( const char* identifier )
{
    SWEET_ASSERT( identifier );
    active_whitespace_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = symbol( identifier );
    return *this;
}

Grammar& Grammar::end_production()
{
    SWEET_ASSERT( active_symbol_ );
    active_whitespace_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::operator()( char literal )
{
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        char lexeme [2] = { literal, 0 };
        whitespace_tokens_.push_back( LexerToken(TOKEN_LITERAL, 0, nullptr, lexeme) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(literal) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( symbol(literal) );
    }
    return *this;
}

Grammar& Grammar::operator()( const char* regex )
{
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( LexerToken(TOKEN_REGULAR_EXPRESSION, 0, nullptr, regex) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(regex) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( symbol(regex) );
    }
    return *this;
}

Grammar& Grammar::operator()( const GrammarNil& /*nil*/ )
{
    SWEET_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
    }
    return *this;
}

Grammar& Grammar::error()
{
    if ( active_directive_ )
    {
        active_directive_->append_symbol( error_symbol() );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        active_production_->append_symbol( error_symbol() );
    }
    return *this;
}

Grammar& Grammar::operator[]( const char* identifier )
{
    SWEET_ASSERT( active_production_ );
    if ( active_production_ )
    {
        active_production_->set_action( action(identifier) );
        active_production_ = nullptr;
    }
    return *this;
}

Grammar& Grammar::operator[]( const GrammarNil& /*nil*/ )
{
    active_production_ = nullptr;
    return *this;
}

void Grammar::end()
{
    int precedence = 1;
    for ( auto i = directives_.begin(); i != directives_.end(); ++i )
    {
        const GrammarDirective* directive = i->get();
        SWEET_ASSERT( directive );
        const vector<GrammarSymbol*>& symbols = directive->symbols();
        for ( auto j = symbols.begin(); j != symbols.end(); ++j )
        {
            GrammarSymbol* symbol = *j;
            SWEET_ASSERT( symbol );
            symbol->set_associativity( directive->associativity() );
            symbol->set_precedence( precedence );
        }
        ++precedence;
    }

    for ( auto i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        GrammarSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( !symbol->productions().empty() )
        {
            symbol->set_lexeme_type( LEXEME_NULL );
            symbol->set_symbol_type( SYMBOL_NON_TERMINAL );
        }
    }
}

GrammarDirective* Grammar::directive( Associativity associativity )
{
    shared_ptr<GrammarDirective> directive( new GrammarDirective(associativity) );
    directives_.push_back( directive );
    return directive.get();
}

GrammarSymbol* Grammar::symbol( char literal )
{
    char lexeme [2] = { literal, 0 };
    return Grammar::symbol( lexeme, LEXEME_LITERAL, SYMBOL_NULL );
}

GrammarSymbol* Grammar::symbol( const char* regex )
{
    SWEET_ASSERT( regex );
    return symbol( regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL );
}

GrammarSymbol* Grammar::error_symbol()
{
    return symbol( ".error", LEXEME_LITERAL, SYMBOL_NULL );
}

GrammarSymbol* Grammar::symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type )
{
    SWEET_ASSERT( lexeme );
    vector<shared_ptr<GrammarSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->lexeme() != lexeme )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        shared_ptr<GrammarSymbol> symbol( new GrammarSymbol(lexeme) );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        symbols_.push_back( symbol );
        return symbol.get();
    }
    GrammarSymbol* symbol = i->get();
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( symbol->lexeme_type() == lexeme_type );
    SWEET_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

GrammarProduction* Grammar::production( GrammarSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    shared_ptr<GrammarProduction> production( new GrammarProduction(symbol) );
    productions_.push_back( production );
    symbol->append_production( production.get() );
    return production.get();
}

GrammarAction* Grammar::action( const char* identifier )
{
    SWEET_ASSERT( identifier );
    vector<shared_ptr<GrammarAction>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        shared_ptr<GrammarAction> action( new GrammarAction(identifier) );
        actions_.push_back( action );
        return action.get();
    }
    return i->get();
}
