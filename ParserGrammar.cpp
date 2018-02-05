// 
// ParserGrammar.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserGrammar.hpp"
#include "ParserGenerator.hpp"
#include "ParserProduction.hpp"
#include "ParserAction.hpp"
#include "GrammarSymbol.hpp"
#include "ErrorCode.hpp"
#include "assert.hpp"
#include <memory>
#include <stdio.h>

using std::set;
using std::vector;
using std::shared_ptr;
using namespace sweet;
using namespace sweet::lalr;
using namespace sweet::lalr;

/**
// Constructor.
//
// @param actions_reserve
//  The number of actions to reserve space for in this ParserGrammar.
//
// @param productions_reserve
//  The number of productions to reserve space for in this ParserGrammar.
//
// @param symbols_reserve
//  The number of symbols to reserve space for in this ParserGrammar.
*/
ParserGrammar::ParserGrammar( size_t actions_reserve, size_t productions_reserve, size_t symbols_reserve )
: identifier_(),
  actions_(),
  productions_(),
  symbols_(),
  start_symbol_( NULL ),
  end_symbol_( NULL ),
  error_symbol_( NULL )
{
    actions_.reserve( actions_reserve );
    productions_.reserve( productions_reserve );
    symbols_.reserve( symbols_reserve );
    start_symbol_ = add_non_terminal( ".start", 0 );
    end_symbol_ = add_symbol( SYMBOL_END, ".end", 0 );
    error_symbol_ = add_terminal( ".error", 0 );
}

ParserGrammar::~ParserGrammar()
{
    for ( auto i = actions_.begin(); i != actions_.end(); ++i )
    {
        ParserAction* action = i->get();
        SWEET_ASSERT( action );
        action->destroy();
    }
}

/**
// Get the identifier of this ParserGrammar.
//
// @return
//  The identifier.
*/
std::string& ParserGrammar::identifier()
{
    return identifier_;
}

/**
// Get the actions in this ParserGrammar.
//
// @return
//  The actions.
*/
std::vector<std::unique_ptr<ParserAction> >& ParserGrammar::actions()
{
    return actions_;
}

/**
// Get the productions in this ParserGrammar.
//
// @return
//  The productions.
*/
std::vector<std::shared_ptr<ParserProduction> >& ParserGrammar::productions()
{
    return productions_;
}

/**
// Get the symbols in this ParserGrammar.
//
// @return
//  The symbols.
*/
std::vector<std::shared_ptr<ParserSymbol> >& ParserGrammar::symbols()
{
    return symbols_;
}

/**
// Get the start symbol in this ParserGrammar.
//
// @return
//  The start symbol.
*/
ParserSymbol* ParserGrammar::start_symbol() const
{
    return start_symbol_;
}

/**
// Get the end symbol in this ParserGrammar.
//
// @return
//  The end symbol.
*/
ParserSymbol* ParserGrammar::end_symbol() const
{
    return end_symbol_;
}

/**
// Get the error symbol in this ParserGrammar.
//
// @return
//  The error symbol.
*/
ParserSymbol* ParserGrammar::error_symbol() const
{
    return error_symbol_;
}

ParserSymbol* ParserGrammar::symbol( const GrammarSymbol* grammar_symbol )
{
    SWEET_ASSERT( grammar_symbol );
    SymbolType type = grammar_symbol->lexeme_type() == LEXEME_NULL ? SYMBOL_NON_TERMINAL : SYMBOL_TERMINAL;
    ParserSymbol* symbol = ParserGrammar::symbol( type, grammar_symbol->lexeme(), 0 );
    symbol->set_associativity( grammar_symbol->associativity() );
    symbol->set_precedence( grammar_symbol->precedence() );
    return symbol;
}

ParserSymbol* ParserGrammar::symbol( SymbolType type, const std::string& identifier, int line )
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

ParserSymbol* ParserGrammar::terminal( const std::string& identifier, int line )
{
    return symbol( SYMBOL_TERMINAL, identifier, line );
}

ParserSymbol* ParserGrammar::non_terminal( const std::string& identifier, int line )
{
    return symbol( SYMBOL_NON_TERMINAL, identifier, line );
}

ParserAction* ParserGrammar::action( const std::string& identifier )
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

/**
// Add a symbol to this ParserGrammar.
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
ParserSymbol* ParserGrammar::add_symbol( SymbolType type, const std::string& identifier, int line )
{
    std::shared_ptr<ParserSymbol> symbol( new ParserSymbol(type, identifier, line) );
    symbols_.push_back( symbol );
    return symbol.get();
}

/**
// Add a terminal symbol to this ParserGrammar.
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
ParserSymbol* ParserGrammar::add_terminal( const std::string& identifier, int line )
{
    return add_symbol( SYMBOL_TERMINAL, identifier, line );
}

/**
// Add a non terminal symbol to this ParserGrammar.
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
ParserSymbol* ParserGrammar::add_non_terminal( const std::string& identifier, int line )
{
    return add_symbol( SYMBOL_NON_TERMINAL, identifier, line );
}

/**
// Add an action to this ParserGrammar.
//
// @param identifier
//  The identifier of the action to add.
//
// @return
//  The action.
*/
ParserAction* ParserGrammar::add_action( const std::string& identifier )
{
    SWEET_ASSERT( !identifier.empty() );    
    ParserAction* action = nullptr;
    if ( !identifier.empty() )
    {
        std::vector<std::unique_ptr<ParserAction> >::const_iterator i = actions_.begin();
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
            std::unique_ptr<ParserAction> new_action( new ParserAction(int(actions_.size()), identifier.c_str()) );
            actions_.push_back( move(new_action) );
            action = actions_.back().get();
        }
    }
    return action;
}

/**
// Set the identifier for this ParserGrammar (optional).
//
// @param identifier
//  The identifier for this ParserGrammar.
*/
void ParserGrammar::identifier( const std::string& identifier )
{
    identifier_ = identifier;
}

/**
// Start a production in this ParserGrammar.
//
// @param symbol
//  The symbol on the left hand side of the production (assumed not null).
//
// @param line
//  The line that the production starts on.
*/
void ParserGrammar::begin_production( ParserSymbol* symbol, int line )
{
    if ( productions_.empty() )
    {
        SWEET_ASSERT( start_symbol_ );
        SWEET_ASSERT( end_symbol_ );

        std::shared_ptr<ParserProduction> production( new ParserProduction(int(productions_.size()), start_symbol_, 0, NULL) );
        productions_.push_back( production );
        ParserGrammar::symbol( symbol );
        start_symbol_->append_production( production );        
    }

    SWEET_ASSERT( symbol );
    std::shared_ptr<ParserProduction> production( new ParserProduction(int(productions_.size()), symbol, line, NULL) );
    productions_.push_back( production );
    symbol->append_production( production );
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
void ParserGrammar::end_production()
{
}

/**
// Append a symbol node to the current production's right hand side.
//
// @param symbol
//  The symbol to append on the right hand side (assumed not null).
*/
void ParserGrammar::symbol( ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( !productions_.empty() );
    ParserProduction* production = productions_.back().get();
    production->append_symbol( symbol );
}

/**
// Set the action to be taken when the current production is reduced.
//
// @param action
//  The action to take when the current production is reduced.
*/
void ParserGrammar::action( ParserAction* action )
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
void ParserGrammar::precedence_symbol( ParserSymbol* symbol )
{
    SWEET_ASSERT( !productions_.empty() );
    productions_.back()->set_precedence_symbol( symbol );
}

/**
// Print this grammar to stdout.
*/
void ParserGrammar::print() const
{
    for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        const ParserSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        if ( symbol->get_type() == SYMBOL_NON_TERMINAL )
        {
            printf( "%s:\n", symbol->description().c_str() );        
            printf( "  nullable=%s\n", symbol->is_nullable() ? "true" : "false" );
            std::string description;
            description.reserve( 1024 );
            ParserSymbol::describe( symbol->get_first(), &description );
            printf( "  first=(%s)\n", description.c_str() );
            description.clear();
            ParserSymbol::describe( symbol->get_follow(), &description );
            printf( "  follow=(%s)\n", description.c_str() );
            printf( "\n\n" );
        }
    }

    for ( vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        const ParserProduction* production = i->get();
        SWEET_ASSERT( production );
        printf( "%s:\n", production->description().c_str() );
        printf( "\n\n" );
    }
}

/**
// Print the nodes in \e positions.
//
// @param positions
//  The nodes that appear after the dot in a production.
*/
void ParserGrammar::print_positions( const std::set<int>& positions ) const
{
    set<int>::const_iterator i = positions.begin();
    if ( i != positions.end() )
    {
        printf( "%d", *i );
        ++i;
    }
    while ( i != positions.end() )
    {
        printf( ", %d", *i );
        ++i;
    }
}

/**
// Calculate identifiers for all symbols.
*/
void ParserGrammar::calculate_identifiers()
{
    for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        ParserSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        symbol->calculate_identifier();
    }
}

/**
// Replace references to \e to_symbol with references to \e with_symbol.
//
// @param to_symbol
//  The ParserSymbol to replace references to.
//
// @param with_symbol
//  The ParserSymbol to replace references with.
*/
void ParserGrammar::replace_references_to_symbol( ParserSymbol* to_symbol, ParserSymbol* with_symbol )
{
    for ( std::vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        ParserProduction* production = i->get();
        SWEET_ASSERT( production );
        production->replace_references_to_symbol( to_symbol, with_symbol );
    }
}

/**
// Calculate which symbols are terminal and non-terminal.
//
// Any symbols with one or more productions are assumed to be non-terminals
// and any symbols with no productions are assumed to be terminals.  Another
// pass is made over the symbols in to convert non-terminals symbols that 
// contain only a single production with one terminal symbol into terminals.
// See `ParserGrammar::calculate_implicit_terminal_symbols()`.
//
// The `.start`, `.end`, and `.error` symbols are exempt from the above 
// processing.  They are explicitly assigned their corr
*/
void ParserGrammar::calculate_terminal_and_non_terminal_symbols()
{
    for ( vector<shared_ptr<ParserSymbol>>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        ParserSymbol* symbol = i->get();
        if ( symbol->get_type() == SYMBOL_NULL )
        {
            symbol->set_type( symbol->get_productions().empty() ? SYMBOL_TERMINAL : SYMBOL_NON_TERMINAL );
        }
    }
}

/**
// Calculate the non terminal symbols that are really just named terminals.
//
// Any symbols that contain a single production that contains only a terminal 
// symbol are really just acting as names for that terminal symbol.  To make 
// the parser easier to understand and more efficient these symbols are 
// collapsed by making any references to the non terminal symbol refer directly
// to the terminal symbol.  The identifier of the terminal is changed to be 
// the more readable name of the non terminal.
//
// For example the rule 'integer: "[0-9]+";' creates a non terminal
// symbol 'integer' and a terminal symbol '"[0-9]+"'.  The non terminal
// symbol 'integer' is redundant from the point of view of the parser as it
// adds only a trivial reduction from one symbol type to another.  To optimize
// this situation the terminal is collapsed into the non terminal keeping the
// more readable name of the non terminal but removing the redundant 
// reduction.
*/
void ParserGrammar::calculate_implicit_terminal_symbols()
{
    for ( vector<std::shared_ptr<ParserSymbol> >::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        ParserSymbol* non_terminal_symbol = i->get();        
        if ( non_terminal_symbol && non_terminal_symbol != error_symbol_ )
        {
            ParserSymbol* terminal_symbol = non_terminal_symbol->get_implicit_terminal();
            if ( terminal_symbol )
            {       
                SWEET_ASSERT( terminal_symbol != non_terminal_symbol );
                terminal_symbol->replace_by_non_terminal( non_terminal_symbol );
                replace_references_to_symbol( non_terminal_symbol, terminal_symbol );
                i->reset();
            }
        }
    }
    
    vector<std::shared_ptr<ParserSymbol> >::iterator i = symbols_.begin();
    while ( i != symbols_.end() )
    {
        if ( !i->get() )
        {
            i = symbols_.erase( i );
        }
        else        
        {
            ++i;
        }
    }
}

/**
// Calculate the first position sets for each ParserSymbol until no more 
// terminals can be added to any first position sets.
*/
void ParserGrammar::calculate_first()
{
    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<std::shared_ptr<ParserSymbol> >::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            ParserSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            added += symbol->calculate_first();
        }
    }
}

/**
// Calculate the follow position sets for each ParserSymbol until no more 
// terminals can be added to any follow position sets.
*/
void ParserGrammar::calculate_follow()
{
    start_symbol_->add_symbol_to_follow( end_symbol_ );

    int added = 1;
    while ( added > 0 )
    {
        added = 0;
        for ( vector<std::shared_ptr<ParserSymbol> >::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            ParserSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            added += symbol->calculate_follow();
        }
    }
}

/**
// Calculate the index for each symbol.
*/
void ParserGrammar::calculate_indices()
{
    int index = 0;
    for ( vector<std::shared_ptr<ParserSymbol> >::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        ParserSymbol* symbol = i->get();
        SWEET_ASSERT( symbol );
        symbol->set_index( index );
        ++index;
    }
}

/**
// Calculate the precedence of each production that hasn't had precedence
// set explicitly as the precedence of its rightmost terminal.
*/
void ParserGrammar::calculate_precedence_of_productions()
{
    for ( vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
    {
        ParserProduction* production = i->get();
        SWEET_ASSERT( production );       
        if ( production->get_precedence() == 0 )
        {
            const ParserSymbol* symbol = production->find_rightmost_terminal_symbol();
            if ( symbol )
            {
                production->set_precedence_symbol( symbol );
            }
        }
    }
}

/**
// Check for symbols in the grammar that are referenced but never defined.
*/
void ParserGrammar::check_for_undefined_symbol_errors( ParserGenerator* generator )
{
    SWEET_ASSERT( generator );

    if ( generator->errors() == 0 )
    {
        for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const ParserSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            if ( symbol->get_type() == SYMBOL_NON_TERMINAL && symbol->get_productions().empty() )
            {
                generator->fire_error( 1, PARSER_ERROR_UNDEFINED_SYMBOL, "Undefined symbol '%s' in grammar '%s'", symbol->get_identifier().c_str(), identifier_.c_str() );
            }
        }
    }
}

/**
// Check for symbols in the grammar that are defined but never referenced.
//
// @param generator
//  The ParserGenerator for fire any errors from (assumed not null).
*/
void ParserGrammar::check_for_unreferenced_symbol_errors( ParserGenerator* generator )
{
    SWEET_ASSERT( generator );

    if ( generator->errors() == 0 )
    {
        for ( vector<std::shared_ptr<ParserSymbol> >::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
        {
            const ParserSymbol* symbol = i->get();
            SWEET_ASSERT( symbol );
            
            int references = 0;            
            if ( symbol != start_symbol_ && symbol != end_symbol_ && symbol != error_symbol_ )
            {
                for ( vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions_.begin(); i != productions_.end(); ++i )
                {
                    const ParserProduction* production = i->get();
                    SWEET_ASSERT( production );
                    if ( production->get_symbol()->get_type() != SYMBOL_TERMINAL )
                    {
                        references += production->count_references_to_symbol( symbol );
                    }
                }

                if ( references == 0 )
                {
                    generator->fire_error( 1, PARSER_ERROR_UNREFERENCED_SYMBOL, "Unreferenced symbol '%s'/'%s'", symbol->get_identifier().c_str(), symbol->get_lexeme().c_str() );
                }
            }
        }
    }
}

/**
// Check for the error symbol being used in the left hand side of a 
// production.
//
// @param generator
//  The ParserGenerator for fire any errors from (assumed not null).
*/
void ParserGrammar::check_for_error_symbol_on_left_hand_side_errors( ParserGenerator* generator )
{
    SWEET_ASSERT( error_symbol_ );
    SWEET_ASSERT( generator );

    const vector<std::shared_ptr<ParserProduction> >& productions = error_symbol_->get_productions();
    for ( vector<std::shared_ptr<ParserProduction> >::const_iterator i = productions.begin(); i != productions.end(); ++i )
    {
        generator->fire_error( 1, PARSER_ERROR_ERROR_SYMBOL_ON_LEFT_HAND_SIDE, "The 'error' symbol appears on the left hand side of a production" );
    }
}
