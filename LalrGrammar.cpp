// 
// LalrGrammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LalrGrammar.hpp"
#include "LalrGenerator.hpp"
#include "LalrProduction.hpp"
#include "LalrAction.hpp"
#include "GrammarSymbol.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"
#include <memory>
#include <stdio.h>

using std::set;
using std::vector;
using std::unique_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

/**
// Constructor.
//
// @param actions_reserve
//  The number of actions to reserve space for in this LalrGrammar.
//
// @param productions_reserve
//  The number of productions to reserve space for in this LalrGrammar.
//
// @param symbols_reserve
//  The number of symbols to reserve space for in this LalrGrammar.
//
// @param whitespace_tokens_reserve
//  The number of whitespace tokens to reserve space for in this LalrGrammar.
*/
LalrGrammar::LalrGrammar( size_t actions_reserve, size_t productions_reserve, size_t symbols_reserve, size_t whitespace_tokens_reserve )
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  whitespace_tokens_(),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL )
{
    actions_.reserve( actions_reserve );
    productions_.reserve( productions_reserve );
    symbols_.reserve( symbols_reserve );
    whitespace_tokens_.reserve( whitespace_tokens_reserve );
    start_symbol_ = add_non_terminal( ".start", 0 );
    end_symbol_ = add_symbol( SYMBOL_END, ".end", 0 );
    error_symbol_ = add_terminal( ".error", 0 );
}

LalrGrammar::~LalrGrammar()
{
    for ( auto i = actions_.begin(); i != actions_.end(); ++i )
    {
        LalrAction* action = i->get();
        SWEET_ASSERT( action );
        action->destroy();
    }
}

/**
// Get the identifier of this LalrGrammar.
//
// @return
//  The identifier.
*/
std::string& LalrGrammar::identifier()
{
    return identifier_;
}

/**
// Get the actions in this LalrGrammar.
//
// @return
//  The actions.
*/
std::vector<std::unique_ptr<LalrAction> >& LalrGrammar::actions()
{
    return actions_;
}

/**
// Get the productions in this LalrGrammar.
//
// @return
//  The productions.
*/
std::vector<std::unique_ptr<LalrProduction> >& LalrGrammar::productions()
{
    return productions_;
}

/**
// Get the symbols in this LalrGrammar.
//
// @return
//  The symbols.
*/
std::vector<std::unique_ptr<LalrSymbol> >& LalrGrammar::symbols()
{
    return symbols_;
}

/**
// Get the tokens skipped as whitespace by the lexer.
//
// @return 
//  The tokens skipped as whitespace.
*/
const std::vector<LexerToken>& LalrGrammar::whitespace_tokens() const
{
    return whitespace_tokens_;
}

/**
// Get the start symbol in this LalrGrammar.
//
// @return
//  The start symbol.
*/
LalrSymbol* LalrGrammar::start_symbol() const
{
    return start_symbol_;
}

/**
// Get the end symbol in this LalrGrammar.
//
// @return
//  The end symbol.
*/
LalrSymbol* LalrGrammar::end_symbol() const
{
    return end_symbol_;
}

/**
// Get the error symbol in this LalrGrammar.
//
// @return
//  The error symbol.
*/
LalrSymbol* LalrGrammar::error_symbol() const
{
    return error_symbol_;
}

LalrSymbol* LalrGrammar::symbol( const GrammarSymbol* grammar_symbol )
{
    SWEET_ASSERT( grammar_symbol );
    SymbolType type = grammar_symbol->lexeme_type() == LEXEME_NULL ? SYMBOL_NON_TERMINAL : SYMBOL_TERMINAL;
    LalrSymbol* symbol = LalrGrammar::symbol( type, grammar_symbol->lexeme(), 0 );
    symbol->set_associativity( grammar_symbol->associativity() );
    symbol->set_precedence( grammar_symbol->precedence() );
    symbol->set_lexeme_type( grammar_symbol->lexeme_type() );
    return symbol;
}

LalrSymbol* LalrGrammar::symbol( SymbolType type, const std::string& identifier, int line )
{
    auto i = symbols_.begin();
    while ( i != symbols_.end() && (*i)->get_lexeme() != identifier )
    {
        ++i;
    }
    if ( i == symbols_.end() )
    {
        return add_symbol( type, identifier, line );
    }
    SWEET_ASSERT( i != symbols_.end() );
    return i->get();
}

LalrSymbol* LalrGrammar::terminal( const std::string& identifier, int line )
{
    return symbol( SYMBOL_TERMINAL, identifier, line );
}

LalrSymbol* LalrGrammar::non_terminal( const std::string& identifier, int line )
{
    return symbol( SYMBOL_NON_TERMINAL, identifier, line );
}

LalrAction* LalrGrammar::action( const std::string& identifier )
{
    auto i = actions_.begin();
    while ( i != actions_.end() && (*i)->identifier != identifier )
    {
        ++i;
    }
    if ( i == actions_.end() )
    {
        return add_action( identifier );
    }
    SWEET_ASSERT( i != actions_.end() );
    return i->get();
}

void LalrGrammar::whitespace_tokens( const std::vector<LexerToken>& whitespace_tokens )
{
    whitespace_tokens_ = whitespace_tokens;
}

/**
// Add a symbol to this LalrGrammar.
//
// @param type
//  The type of symbol to add.
//
// @param identifier
//  The identifier to use for the symbol (for debugging purposes only).
//
// @param line
//  The line that the symbol first appears on (for debugging purposes only).
//
// @return
//  The symbol.
*/
LalrSymbol* LalrGrammar::add_symbol( SymbolType type, const std::string& identifier, int line )
{
    unique_ptr<LalrSymbol> symbol( new LalrSymbol(type, identifier, line) );
    symbols_.push_back( move(symbol) );
    return symbols_.back().get();
}

/**
// Add a terminal symbol to this LalrGrammar.
//
// @param identifier
//  The identifier to use for the symbol (for debugging purposes only).
//
// @param line
//  The line that the symbol first appears on (for debugging purposes only).
//
// @return
//  The terminal symbol.
*/
LalrSymbol* LalrGrammar::add_terminal( const std::string& identifier, int line )
{
    return add_symbol( SYMBOL_TERMINAL, identifier, line );
}

/**
// Add a non terminal symbol to this LalrGrammar.
//
// @param identifier
//  The identifier to use for the symbol (for debugging purposes only).
//
// @param line
//  The line that the symbol first appears on (for debugging purposes only).
//
// @return
//  The terminal symbol.
*/
LalrSymbol* LalrGrammar::add_non_terminal( const std::string& identifier, int line )
{
    return add_symbol( SYMBOL_NON_TERMINAL, identifier, line );
}

/**
// Add an action to this LalrGrammar.
//
// @param identifier
//  The identifier of the action to add.
//
// @return
//  The action.
*/
LalrAction* LalrGrammar::add_action( const std::string& identifier )
{
    SWEET_ASSERT( !identifier.empty() );    
    LalrAction* action = nullptr;
    if ( !identifier.empty() )
    {
        std::vector<std::unique_ptr<LalrAction> >::const_iterator i = actions_.begin();
        while ( i != actions_.end() && (*i)->identifier != identifier )
        {
            ++i;
        }        
        if ( i != actions_.end() )
        {
            action = i->get();
        }
        else
        {
            std::unique_ptr<LalrAction> new_action( new LalrAction(int(actions_.size()), identifier.c_str()) );
            actions_.push_back( move(new_action) );
            action = actions_.back().get();
        }
    }
    return action;
}

/**
// Set the identifier for this LalrGrammar (optional).
//
// @param identifier
//  The identifier for this LalrGrammar.
*/
void LalrGrammar::identifier( const std::string& identifier )
{
    identifier_ = identifier;
}

/**
// Start a production in this LalrGrammar.
//
// @param symbol
//  The symbol on the left hand side of the production (assumed not null).
//
// @param line
//  The line that the production starts on.
*/
void LalrGrammar::begin_production( LalrSymbol* symbol, int line )
{
    if ( productions_.empty() )
    {
        SWEET_ASSERT( start_symbol_ );
        SWEET_ASSERT( end_symbol_ );

        unique_ptr<LalrProduction> production( new LalrProduction(int(productions_.size()), start_symbol_, 0, NULL) );
        start_symbol_->append_production( production.get() );
        productions_.push_back( move(production) );
        LalrGrammar::symbol( symbol );
    }

    SWEET_ASSERT( symbol );
    unique_ptr<LalrProduction> production( new LalrProduction(int(productions_.size()), symbol, line, NULL) );
    symbol->append_production( production.get() );
    productions_.push_back( move(production) );
}

/**
// End a production.
//
// If there is more than one node then this end production has been matched as
// part of a top level '|' expression and the right hand node is really the 
// first symbol in another production for the same symbol as the current 
// production.  It will be reduced later when another top level '|' expression
// or the ';' at the end of the production is matched.
*/
void LalrGrammar::end_production()
{
}

/**
// Append a symbol node to the current production's right hand side.
//
// @param symbol
//  The symbol to append on the right hand side (assumed not null).
*/
void LalrGrammar::symbol( LalrSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( !productions_.empty() );
    LalrProduction* production = productions_.back().get();
    production->append_symbol( symbol );
}

/**
// Set the action to be taken when the current production is reduced.
//
// @param action
//  The action to take when the current production is reduced.
*/
void LalrGrammar::action( LalrAction* action )
{
    SWEET_ASSERT( !productions_.empty() );
    productions_.back()->set_action( action );
}

/**
// Set the precedence of the current production to match the precedence
// give to \e symbol.
//
// @param symbol
//  The symbol to set the precedence of the current production to match 
//  (assumed not null).
*/
void LalrGrammar::precedence_symbol( LalrSymbol* symbol )
{
    SWEET_ASSERT( !productions_.empty() );
    productions_.back()->set_precedence_symbol( symbol );
}
