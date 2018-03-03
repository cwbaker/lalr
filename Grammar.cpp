//
// Grammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "Grammar.hpp"
#include "Directive.hpp"
#include "Symbol.hpp"
#include "Production.hpp"
#include "Action.hpp"
#include "Generator.hpp"
#include "Action.hpp"
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
  active_whitespace_directive_( false ),
  active_precedence_directive_( false ),
  active_directive_( nullptr ),
  active_production_( nullptr ),
  active_symbol_( nullptr ),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL )
{
    start_symbol_ = symbol( ".start", LEXEME_NULL, SYMBOL_NON_TERMINAL );
    end_symbol_ = symbol( ".end", LEXEME_NULL, SYMBOL_END );
    error_symbol_ = symbol( ".error", LEXEME_NULL, SYMBOL_NULL );
}

Grammar::~Grammar()
{
}

const std::string& Grammar::identifier() const
{
    return identifier_;
}

std::vector<std::unique_ptr<Directive>>& Grammar::directives()
{
    return directives_;
}

std::vector<std::unique_ptr<Symbol>>& Grammar::symbols()
{
    return symbols_;
}

std::vector<std::unique_ptr<Production>>& Grammar::productions()
{
    return productions_;
}

std::vector<std::unique_ptr<Action>>& Grammar::actions()
{
    return actions_;
}

const std::vector<LexerToken>& Grammar::whitespace_tokens() const
{
    return whitespace_tokens_;
}

Symbol* Grammar::start_symbol() const
{
    return start_symbol_;
}

Symbol* Grammar::end_symbol() const
{
    return end_symbol_;
}

Symbol* Grammar::error_symbol() const
{
    return error_symbol_;
}

Grammar& Grammar::left()
{
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_directive_ = directive( ASSOCIATE_LEFT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::right()
{
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_directive_ = directive( ASSOCIATE_RIGHT );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::none()
{
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_directive_ = directive( ASSOCIATE_NONE );
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::whitespace()
{
    active_whitespace_directive_ = true;
    active_precedence_directive_ = false;
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

Grammar& Grammar::precedence()
{
    SWEET_ASSERT( active_symbol_ );
    if ( active_symbol_ )
    {
        active_precedence_directive_ = true;
    }
    return *this;
}

Grammar& Grammar::production( const char* identifier )
{
    SWEET_ASSERT( identifier );
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = symbol( identifier, LEXEME_NULL, SYMBOL_NON_TERMINAL );
    return *this;
}

Grammar& Grammar::end_production()
{
    SWEET_ASSERT( active_symbol_ );
    active_whitespace_directive_ = false;
    active_precedence_directive_ = false;
    active_directive_ = nullptr;
    active_production_ = nullptr;
    active_symbol_ = nullptr;
    return *this;
}

Grammar& Grammar::operator()( char literal )
{
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    char lexeme [2] = { literal, 0 };
    return Grammar::literal( lexeme );
}

Grammar& Grammar::operator()( const char* regex )
{
    return Grammar::regex( regex );
}

Grammar& Grammar::operator()( const Nil& /*nil*/ )
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

Grammar& Grammar::operator[]( const Nil& /*nil*/ )
{
    active_production_ = nullptr;
    return *this;
}

Grammar& Grammar::literal( const char* literal )
{
    SWEET_ASSERT( literal );
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( LexerToken(TOKEN_LITERAL, 0, nullptr, literal) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(literal, LEXEME_LITERAL, SYMBOL_NULL) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }        
        if ( active_precedence_directive_ )
        {
            active_production_->set_precedence_symbol( symbol(literal, LEXEME_LITERAL, SYMBOL_NULL) );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( symbol(literal, LEXEME_LITERAL, SYMBOL_NULL) );
        }
    }
    return *this;
}

Grammar& Grammar::regex( const char* regex )
{
    SWEET_ASSERT( regex );
    SWEET_ASSERT( active_whitespace_directive_ || active_directive_ || active_symbol_ );
    if ( active_whitespace_directive_ )
    {
        whitespace_tokens_.push_back( LexerToken(TOKEN_REGULAR_EXPRESSION, 0, nullptr, regex) );
    }
    else if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        if ( active_precedence_directive_ )
        {
            active_production_->set_precedence_symbol( symbol(regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL) );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( symbol(regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL) );
        }
    }
    return *this;
}

Grammar& Grammar::identifier( const char* identifier )
{
    SWEET_ASSERT( identifier );
    SWEET_ASSERT( active_directive_ || active_symbol_ );
    if ( active_directive_ )
    {
        active_directive_->append_symbol( symbol(identifier, LEXEME_NULL, SYMBOL_NON_TERMINAL) );
    }
    else if ( active_symbol_ )
    {
        if ( !active_production_ )
        {
            active_production_ = production( active_symbol_ );
        }
        if ( active_precedence_directive_ )
        {
            active_production_->set_precedence_symbol( symbol(identifier, LEXEME_NULL, SYMBOL_NON_TERMINAL) );
            active_precedence_directive_ = false;
        }
        else
        {
            active_production_->append_symbol( symbol(identifier, LEXEME_NULL, SYMBOL_NON_TERMINAL) );
        }
    }
    return *this;
}

void Grammar::generate( ParserStateMachine* state_machine, ParserErrorPolicy* parser_error_policy, LexerErrorPolicy* lexer_error_policy )
{
    SWEET_ASSERT( state_machine );

    int precedence = 1;
    for ( auto i = directives_.begin(); i != directives_.end(); ++i )
    {
        const Directive* directive = i->get();
        SWEET_ASSERT( directive );
        const vector<Symbol*>& symbols = directive->symbols();
        for ( auto j = symbols.begin(); j != symbols.end(); ++j )
        {
            Symbol* symbol = *j;
            SWEET_ASSERT( symbol );
            symbol->set_associativity( directive->associativity() );
            symbol->set_precedence( precedence );
        }
        ++precedence;
    }

    for ( auto i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        Symbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( !symbol->productions().empty() )
        {
            symbol->set_lexeme_type( LEXEME_NULL );
            symbol->set_symbol_type( SYMBOL_NON_TERMINAL );
        }
    }

    Generator generator( *this, state_machine, parser_error_policy, lexer_error_policy );
}

Directive* Grammar::directive( Associativity associativity )
{
    unique_ptr<Directive> directive( new Directive(associativity) );
    directives_.push_back( move(directive) );
    return directives_.back().get();
}

Symbol* Grammar::symbol( char literal )
{
    char lexeme [2] = { literal, 0 };
    return Grammar::symbol( lexeme, LEXEME_LITERAL, SYMBOL_NULL );
}

Symbol* Grammar::symbol( const char* regex )
{
    SWEET_ASSERT( regex );
    return symbol( regex, LEXEME_REGULAR_EXPRESSION, SYMBOL_NULL );
}

Symbol* Grammar::symbol( const char* lexeme, LexemeType lexeme_type, SymbolType symbol_type )
{
    SWEET_ASSERT( lexeme );
    vector<unique_ptr<Symbol>>::const_iterator i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->lexeme() != lexeme )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        unique_ptr<Symbol> symbol( new Symbol(lexeme) );
        symbol->set_lexeme_type( lexeme_type );
        symbol->set_symbol_type( symbol_type );
        symbols_.push_back( move(symbol) );
        return symbols_.back().get();
    }
    Symbol* symbol = i->get();
    SWEET_ASSERT( symbol );
    // SWEET_ASSERT( symbol->lexeme_type() == lexeme_type );
    // SWEET_ASSERT( symbol->symbol_type() == symbol_type );
    return symbol;
}

Production* Grammar::production( Symbol* symbol )
{
    SWEET_ASSERT( symbol );
    if ( productions_.empty() )
    {
        SWEET_ASSERT( start_symbol_ );
        unique_ptr<Production> production( new Production(int(productions_.size()), start_symbol_, 0, NULL) );
        production->append_symbol( symbol );
        start_symbol_->append_production( production.get() );
        productions_.push_back( move(production) );
    }

    unique_ptr<Production> production( new Production(int(productions_.size()), symbol, -1, nullptr) );
    symbol->append_production( production.get() );
    productions_.push_back( move(production) );
    return productions_.back().get();
}

Action* Grammar::action( const char* identifier )
{
    SWEET_ASSERT( identifier );
    vector<unique_ptr<Action>>::const_iterator i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier() != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        int index = int(actions_.size());
        unique_ptr<Action> action( new Action(index, identifier) );
        actions_.push_back( move(action) );
        return actions_.back().get();
    }
    return i->get();
}
