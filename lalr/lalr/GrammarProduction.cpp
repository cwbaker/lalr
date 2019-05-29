//
// GrammarProduction.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarProduction.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarAction.hpp"
#include "assert.hpp"

using std::vector;
using namespace lalr;

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
GrammarProduction::GrammarProduction( int index, GrammarSymbol* symbol, int line, int column, const GrammarAction* action )
: index_( index ),
  symbol_( symbol ),
  line_( line ),
  column_( column ),
  action_( action ),
  precedence_symbol_( NULL )
{
    LALR_ASSERT( symbol_ );
}

/**
// Get the index of this production.
//
// @return
//  The index.
*/
int GrammarProduction::index() const
{
    return index_;
}

/**
// Get the symbol on the left-hand side of this production.
//
// @return
//  The symbol.
*/
GrammarSymbol* GrammarProduction::symbol() const
{
    LALR_ASSERT( symbol_ );
    return symbol_;
}

/**
// Get the line in the grammar that this production is defined on.
//
// @return
//  The line.
*/
int GrammarProduction::line() const
{
    return line_;
}

/**
// Get the column in the grammar that this production is defined on.
//
// @return
//  The column.
*/
int GrammarProduction::column() const
{
    return column_;
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
int GrammarProduction::count_references_to_symbol( const GrammarSymbol* symbol ) const
{
    int references = 0;
    for ( vector<GrammarSymbol*>::const_iterator i = symbols_.begin(); i != symbols_.end(); ++i )
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
const GrammarSymbol* GrammarProduction::find_rightmost_terminal_symbol() const
{
    vector<GrammarSymbol*>::const_reverse_iterator i = symbols_.rbegin();
    while ( i != symbols_.rend() && (*i)->symbol_type() != SYMBOL_TERMINAL )
    {
        ++i;
    }
    return i != symbols_.rend() ? *i : NULL;
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
const GrammarSymbol* GrammarProduction::symbol_by_position( int position ) const
{
    return position >= 0 && position < int(symbols_.size()) ? symbols_[position] : NULL;
}

/**
// Get the symbols on the right hand side of this production.
//
// @return
//  The symbols.
*/
const std::vector<GrammarSymbol*>& GrammarProduction::symbols() const
{
    return symbols_;
}

/**
// Get the length of the right-hand side of this production.
//
// @return
//  The length of the right-hand side of this production.
*/
int GrammarProduction::length() const
{
    return int(symbols_.size());
}

/**
// Append \e symbol to the end of the right hand side of this production.
//
// @param symbol
//  The symbol to append (assumed not null).
*/
void GrammarProduction::append_symbol( GrammarSymbol* symbol )
{
    symbols_.push_back( symbol );
}

/**
// Set the action taken when this production is reduced.
//
// @param
//  The action to take when this production is reduced or null to set this
//  production to have no action.
*/
void GrammarProduction::set_action( const GrammarAction* action )
{
    action_ = action;
}

/**
// Get the action taken when this production is reduced.
//
// @return
//  The action or null if this production doesn't have an action.
*/
const GrammarAction* GrammarProduction::action() const
{
    return action_;
}

int GrammarProduction::action_index() const
{
    return action_ ? action_->index() : GrammarAction::INVALID_INDEX;
}

const GrammarSymbol* GrammarProduction::precedence_symbol() const
{
    return precedence_symbol_;
}

/**
// Get the precedence of this production.
//
// @return
//  The precedence of this production.
*/
int GrammarProduction::precedence() const
{
    return precedence_symbol_ ? precedence_symbol_->precedence() : 0;
}

/**
// Set the symbol that this production gets its precedence from.
//
// @param symbol
//  The symbol to have this production inherit its precedence from (assumed 
//  not null).
*/
void GrammarProduction::set_precedence_symbol( const GrammarSymbol* symbol )
{
    LALR_ASSERT( symbol );
    LALR_ASSERT( !precedence_symbol_ );
    precedence_symbol_ = symbol;
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
void GrammarProduction::replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol )
{
    if ( symbol_ == to_symbol )
    {
        symbol_ = with_symbol;
    }
    
    if ( precedence_symbol_ == to_symbol )
    {
        precedence_symbol_ = with_symbol;
    }

    for ( vector<GrammarSymbol*>::iterator i = symbols_.begin(); i != symbols_.end(); ++i )
    {
        if ( *i == to_symbol )
        {
            *i = with_symbol;
        }
    }
}
