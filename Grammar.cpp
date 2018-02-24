//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "LalrDirective.hpp"
#include "LalrSymbol.hpp"
#include "LalrProduction.hpp"
#include "LalrAction.hpp"
#include "LalrGenerator.hpp"
#include "LalrGrammar.hpp"
#include "LalrAction.hpp"
#include "LalrState.hpp"
#include "ParserStateMachine.hpp"
#include "ParserSymbol.hpp"
#include "LexerStateMachine.hpp"
#include "LexerTokenType.hpp"
#include "assert.hpp"

using std::string;
using std::vector;
using std::set;
using std::unique_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

Grammar::Grammar()
: identifier_(),
  directives_(),
  symbols_(),
  productions_(),
  actions_(),
  whitespace_tokens_(),
  active_directive_( nullptr ),
  active_production_( nullptr ),
  active_symbol_( nullptr )
{
}

Grammar::~Grammar()
{
}

const std::vector<std::unique_ptr<LalrDirective>>& Grammar::directives() const
{
    return directives_;
}

const std::vector<std::unique_ptr<LalrSymbol>>& Grammar::symbols() const
{
    return symbols_;
}

const std::vector<std::unique_ptr<LalrProduction>>& Grammar::productions() const
{
    return productions_;
}

const std::vector<std::unique_ptr<LalrAction>>& Grammar::actions() const
{
    return actions_;
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

Grammar& Grammar::operator()( const LalrNil& /*nil*/ )
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

Grammar& Grammar::operator[]( const LalrNil& /*nil*/ )
{
    active_production_ = nullptr;
    return *this;
}

void Grammar::end()
{
}

void Grammar::generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy, LexerErrorPolicy* lexer_error_policy )
{
    SWEET_ASSERT( state_machine );

    int precedence = 1;
    for ( auto i = directives_.begin(); i != directives_.end(); ++i )
    {
        const LalrDirective* directive = i->get();
        SWEET_ASSERT( directive );
        const vector<LalrSymbol*>& symbols = directive->symbols();
        for ( auto j = symbols.begin(); j != symbols.end(); ++j )
        {
            LalrSymbol* symbol = *j;
            SWEET_ASSERT( symbol );
            symbol->set_associativity( directive->associativity() );
            symbol->set_precedence( precedence );
        }
        ++precedence;
    }

    for ( auto i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        LalrSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( !symbol->productions().empty() )
        {
            symbol->set_lexeme_type( LEXEME_NULL );
            symbol->set_symbol_type( SYMBOL_NON_TERMINAL );
        }
    }

    LalrGrammar parser_grammar;

    const vector<unique_ptr<LalrProduction>>& productions = productions_;
    for ( auto i = productions.begin(); i != productions.end(); ++i )
    {
        const LalrProduction* production = i->get();
        SWEET_ASSERT( production );

        const LalrSymbol* symbol = production->symbol();
        SWEET_ASSERT( symbol );

        parser_grammar.begin_production( parser_grammar.make_symbol(symbol), 0 );
        const vector<LalrSymbol*>& symbols = production->symbols();
        for ( auto k = symbols.begin(); k != symbols.end(); ++k )
        {
            parser_grammar.symbol( parser_grammar.make_symbol(*k) );
        }
        const LalrAction* action = production->action();
        if ( action )
        {
           parser_grammar.action( parser_grammar.action(action->identifier()) );
        }
        const LalrSymbol* precedence_symbol = production->precedence_symbol();
        if ( precedence_symbol )
        {
            parser_grammar.precedence_symbol( parser_grammar.make_symbol(precedence_symbol) );
        }
        parser_grammar.end_production();
    }

    parser_grammar.whitespace_tokens( whitespace_tokens_ );

    const vector<unique_ptr<LalrSymbol>>& symbols = symbols_;
    for ( auto i = symbols.begin(); i != symbols.end(); ++i )
    {
        const LalrSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( symbol->lexeme_type() != LEXEME_NULL )
        {
            LalrSymbol* parser_symbol = parser_grammar.make_symbol( symbol );
            parser_symbol->set_associativity( symbol->associativity() );
            parser_symbol->set_precedence( symbol->precedence() );
        }
    }

    LalrGenerator generator( parser_grammar, state_machine, parser_error_policy, lexer_error_policy );
}

LalrDirective* Grammar::directive( Associativity associativity )
{
    unique_ptr<LalrDirective> directive( new LalrDirective(associativity) );
    directives_.push_back( move(directive) );
    return directives_.back().get();
}

LalrSymbol* Grammar::symbol( char literal )
{
    char lexeme [2] = { literal, 0 };
    return Grammar::symbol( lexeme, LEXEME_LITERAL, SYMBOL_NULL );
}

LalrSymbol* Grammar::symbol( const char* regex )
{
    SWEET_ASSERT( regex );
    return symbol( regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL );
}

LalrSymbol* Grammar::error_symbol()
{
    return symbol( ".error", LEXEME_LITERAL, SYMBOL_NULL );
}

LalrSymbol* Grammar::symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type )
{
    SWEET_ASSERT( lexeme );
    vector<unique_ptr<LalrSymbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->lexeme() != lexeme )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        unique_ptr<LalrSymbol> symbol( new LalrSymbol(lexeme) );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        symbols_.push_back( move(symbol) );
        return symbols_.back().get();
    }
    LalrSymbol* symbol = i->get();
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( symbol->lexeme_type() == lexeme_type );
    SWEET_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

LalrProduction* Grammar::production( LalrSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    int index = int(productions_.size());
    unique_ptr<LalrProduction> production( new LalrProduction(index, symbol, -1, nullptr) );
    productions_.push_back( move(production) );
    symbol->append_production( productions_.back().get() );
    return productions_.back().get();
}

LalrAction* Grammar::action( const char* identifier )
{
    SWEET_ASSERT( identifier );
    vector<unique_ptr<LalrAction>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        int index = int(actions_.size());
        unique_ptr<LalrAction> action( new LalrAction(index, identifier) );
        actions_.push_back( move(action) );
        return actions_.back().get();
    }
    return i->get();
}
