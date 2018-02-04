//
// ParserProduction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "ParserProduction.hpp"
#include "ParserSymbol.hpp"
#include "ParserAction.hpp"
#include "assert.hpp"

using std::vector;
using namespace sweet::lalr;

/*
// Constructor.
//
// @param index
//  The index of this production.
//
// @param symbol
//  The symbol on the left-hand side of the production (assumed not null).
//
// @param line
//  The line in the grammar that this production is defnied on.
//
// @param action
//  The action taken when the production is reduced or null if the production
//  has no action.
*/
ParserProduction::ParserProduction( int index, ParserSymbol* symbol, int line, const ParserAction* action )
: index_( index ),
  symbol_( symbol ),
  line_( line ),
  action_( action ),
  precedence_symbol_( NULL )
{
    SWEET_ASSERT( symbol_ );
}

/**
// Get the index of this production.
//
// @return
//  The index.
*/
int ParserProduction::get_index() const
{
    return index_;
}

/**
// Get the symbol on the left-hand side of this production.
//
// @return
//  The symbol.
*/
ParserSymbol* ParserProduction::get_symbol() const
{
    SWEET_ASSERT( symbol_ );
    return symbol_;
}

/**
// Get the line in the grammar that this production is defined on.
//
// @return
//  The line.
*/
int ParserProduction::get_line() const
{
    return line_;
}

/**
// Count the number of references the right-hand side of this production 
// makes to \e symbol.
//
// @param symbol
//  The symbol to count the number of references to.
//
// @return
//  The number of references.
*/
int ParserProduction::count_references_to_symbol( const ParserSymbol* symbol ) const
{
    int references = 0;
    for ( vector<ParserSymbol*>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        references += (symbol == *i ? 1 : 0);
    }
    references += (precedence_symbol_ == symbol ? 1 : 0);
    return references;
}

/**
// Find the rightmost terminal symbol in this production.
//
// If there is more than one possible rightmost terminal symbol because of 
// an embedded or operators then this function returns null.  In this case the precedence
// and associativity of this production must be specified explicitly.
//
// @return
//  The rightmost terminal symbol or null if there was more than one potential
//  rightmost terminal symbol.
*/
const ParserSymbol* ParserProduction::find_rightmost_terminal_symbol() const
{
    vector<ParserSymbol*>::const_reverse_iterator i = symbols_.rbegin();
    while ( i != symbols_.rend() && (*i)->get_type() != SYMBOL_TERMINAL )
    {
        ++i;
    }
    return i != symbols_.rend() ? *i : NULL;
}

/**
// Get the length of the right-hand side of this production.
//
// @return
//  The length of the right-hand side of this production.
*/
int ParserProduction::get_length() const
{
    return int(symbols_.size());
}

/**
// Get a description of this production.
//
// @return
//  The description.
*/
std::string ParserProduction::description() const
{
    std::string description;
    description.reserve( 1024 );
    describe( &description );
    return description;
}

/**
// Describe this production.
//
// @param description
//  A variable to receive the description of this production.
*/
void ParserProduction::describe( std::string* description ) const
{
    SWEET_ASSERT( description );
    symbol_->describe( description );
    description->append( " <- " );
    for ( vector<ParserSymbol*>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        const ParserSymbol* symbol = *i;
        SWEET_ASSERT( symbol );
        symbol->describe( description );
        description->append( " " );        
    }
}

/**
// Append \e symbol to the end of the right hand side of this production.
//
// @param symbol
//  The symbol to append (assumed not null).
*/
void ParserProduction::append_symbol( ParserSymbol* symbol )
{
    symbols_.push_back( symbol );
}

/**
// Get the symbol at \e position on the right hand side of this production.
//
// @param position
//  The position to get the symbol from.
//
// @return
//  The symbol at \e position or null if \e position refers past the end of
//  this production.
*/
const ParserSymbol* ParserProduction::get_symbol_by_position( int position ) const
{
    return position >= 0 && position < int(symbols_.size()) ? symbols_[position] : NULL;
}

/**
// Get the symbols on the right hand side of this production.
//
// @return
//  The symbols.
*/
const std::vector<ParserSymbol*>& ParserProduction::get_symbols() const
{
    return symbols_;
}

/**
// Set the action taken when this production is reduced.
//
// @param
//  The action to take when this production is reduced or null to set this
//  production to have no action.
*/
void ParserProduction::set_action( const ParserAction* action )
{
    action_ = action;
}

/**
// Get the action taken when this production is reduced.
//
// @return
//  The action or null if this production doesn't have an action.
*/
const ParserAction* ParserProduction::get_action() const
{
    return action_;
}

int ParserProduction::action_index() const
{
    return action_ ? action_->index : ParserAction::INVALID_INDEX;
}

/**
// Describe the productions in \e productions.
//
// @param productions
//  The productions to describe.
//
// @param description
//  A variable to receive the description of the productions.
*/
void ParserProduction::describe( const std::set<const ParserProduction*>& productions, std::string* description )
{
    SWEET_ASSERT( description );

    std::set<const ParserProduction*>::const_iterator i = productions.begin(); 
    if ( i != productions.end() )
    {
        const ParserProduction* production = *i;
        SWEET_ASSERT( production );        
        production->describe( description );
        ++i;        
    }
    
    while ( i != productions.end() )
    {
        const ParserProduction* production = *i;
        SWEET_ASSERT( production );
        description->append( ", " );
        production->describe( description );
        ++i;        
    }
}

/**
// Set the symbol that this production gets its precedence from.
//
// @param symbol
//  The symbol to have this production inherit its precedence from (assumed 
//  not null).
*/
void ParserProduction::set_precedence_symbol( const ParserSymbol* symbol )
{
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( !precedence_symbol_ );
    precedence_symbol_ = symbol;
}

/**
// Get the precedence of this production.
//
// @return
//  The precedence of this production.
*/
int ParserProduction::get_precedence() const
{
    return precedence_symbol_ ? precedence_symbol_->get_precedence() : 0;
}

/**
// Replace references to \e to_symbol in this production by \e with_symbol.
//
// @param to_symbol
//  The symbol to replace references to.
//
// @param with_symbol
//  The symbol to replace the references with.
*/
void ParserProduction::replace_references_to_symbol( ParserSymbol* to_symbol, ParserSymbol* with_symbol )
{
    if ( symbol_ == to_symbol )
    {
        symbol_ = with_symbol;
    }
    
    if ( precedence_symbol_ == to_symbol )
    {
        precedence_symbol_ = with_symbol;
    }

    for ( vector<ParserSymbol*>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        if ( *i == to_symbol )
        {
            *i = with_symbol;
        }
    }
}
