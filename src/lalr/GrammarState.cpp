//
// GrammarState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarState.hpp"
#include "GrammarItem.hpp"
#include "GrammarTransition.hpp"
#include "assert.hpp"
#include <stdio.h>

using std::set;
using namespace lalr;

/**
// Constructor.
*/
GrammarState::GrammarState()
: items_()
, transitions_()
, processed_( false )
, index_( INVALID_INDEX )
{
}

GrammarState::GrammarState( const GrammarState& state )
: items_( state.items_ )
, transitions_( state.transitions_ )
, processed_( state.processed_ )
, index_( state.index_ )
{
}

/**
// Get the items in this state.
//
// @return
//  The items.
*/
const std::set<GrammarItem>& GrammarState::items() const
{
    return items_;
}

/**
// Find a transition on \e symbol from this state.
//
// @param symbol
//  The symbol to find a transition for.
//
// @return
//  The transition or null if there is no transition on \e symbol from this
//  state.
*/
const GrammarTransition* GrammarState::find_transition_by_symbol( const GrammarSymbol* symbol ) const
{    
    const GrammarTransition* transition = NULL;
    
    if ( symbol )
    {
        std::set<GrammarTransition>::const_iterator i = transitions_.begin();
        while ( i != transitions_.end() && !i->taken_on_symbol(symbol) )
        {
            ++i;
        }
        transition = i != transitions_.end() ? &(*i) : NULL;
    }
    
    return transition;
}

/**
// Get the transitions from this state.
//
// @return
//  The transitions.
*/
const std::set<GrammarTransition>& GrammarState::transitions() const
{
    return transitions_;
}

/**
// Has this state been processed?
//
// @return
//  True if this state has been processed otherwise false.
*/
bool GrammarState::processed() const
{
    return processed_;
}

/**
// Get the index of this state.
//
// @return
//  The index of this state.
*/
int GrammarState::index() const
{
    return index_;
}

/**
// Less than operator.
//
// @param state
//  The state to compare this state with.
//
// @return
//  True if the items in this state are less than the items in \e state.
*/
bool GrammarState::operator<( const GrammarState& state ) const
{
    return std::lexicographical_compare( items_.begin(), items_.end(), state.items_.begin(), state.items_.end() );
}

/**
// Add an item to this state.
//
// @param production
//  The production of the item to add.
//
// @param position
//  The position of the dot in the item to add.
//
// @return
//  The number of items added (0 or 1).
*/
int GrammarState::add_item( GrammarProduction* production, int position )
{
    LALR_ASSERT( production );
    return items_.insert( GrammarItem(production, position) ).second ? 1 : 0;
}

/**
// Add the symbols in *lookahead_symbols* to the item in this state for 
// *production* at *position*.
//
// @param production
//  The production of the item to add *lookahead_symbols* to.
//
// @param position
//  The position of the dot in the item to add *lookahead_symbols* to.
//
// @param lookahead_symbols
//  The lookahead symbols to add to the item in this state.
//
// @return
//  The number of lookahead symbols added.
*/
int GrammarState::add_lookahead_symbols( GrammarProduction* production, int position, const std::set<const GrammarSymbol*, GrammarSymbolLess>& lookahead_symbols )
{
    LALR_ASSERT( production );
    std::set<GrammarItem>::iterator item = items_.find( GrammarItem(production, position) );
    LALR_ASSERT( item != items_.end() );
    return item->add_lookahead_symbols( lookahead_symbols );
}

/**
// Add a transition on \e symbol from this state to \e state.
//
// @param symbol
//  The symbol to transition on (assumed not null).
//
// @param state
//  The state to transition to (assumed not null).
*/
void GrammarState::add_transition( const GrammarSymbol* symbol, GrammarState* state )
{
    LALR_ASSERT( symbol );
    LALR_ASSERT( state );
    LALR_ASSERT( transitions_.find(GrammarTransition(symbol, state)) == transitions_.end() );
    transitions_.insert( GrammarTransition(symbol, state) );
}

/**
// Add a reduction to \e production from this state on \e symbol.
//
// @param symbol
//  The symbol to make the reduction on.
//
// @param reduced_symbol
//  The symbol that is reduced to.
//
// @param reduced_length
//  The number of symbols on the right-hand side of the production that is
//  reduced.
//
// @param action
//  The index of the action taken on the reduction or 
//  `Action::INVALID_INDEX` if no action is taken.
*/
void GrammarState::add_transition( const GrammarSymbol* symbol, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action )
{
    LALR_ASSERT( symbol );
    LALR_ASSERT( reduced_symbol );
    LALR_ASSERT( reduced_length >= 0 );
    LALR_ASSERT( precedence >= 0 );

    std::set<GrammarTransition>::iterator transition = transitions_.find( GrammarTransition(symbol, reduced_symbol, reduced_length, precedence, action) );
    if ( transition != transitions_.end() )
    {        
        LALR_ASSERT( transition->type() == TRANSITION_SHIFT );
        transition->override_shift_to_reduce( reduced_symbol, reduced_length, precedence, action );
    }
    else
    {
        transition = transitions_.insert( GrammarTransition(symbol, reduced_symbol, reduced_length, precedence, action) ).first;
    }
}

/**
// Add a reduction to \e production from this state on any of the symbols in 
// \e symbols.
//
// @param symbols
//  The symbols to make the reduction on.
//
// @param reduced_symbol
//  The symbol that is reduced to.
//
// @param reduced_length
//  The number of symbols on the right-hand side of the production that is
//  reduced.
//
// @param action
//  The index of the action taken on the reduction or 
//  `Action::INVALID_INDEX` if no action is taken.
*/
void GrammarState::add_transition( const std::set<const GrammarSymbol*, GrammarSymbolLess>& symbols, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action )
{
    LALR_ASSERT( reduced_symbol );
    LALR_ASSERT( reduced_length >= 0 );
    LALR_ASSERT( precedence >= 0 );
    for ( set<const GrammarSymbol*>::const_iterator i = symbols.begin(); i != symbols.end(); ++i )
    {
        const GrammarSymbol* symbol = *i;
        LALR_ASSERT( symbol );
        add_transition( symbol, reduced_symbol, reduced_length, precedence, action );
    }
}

/**
// Find a transition on \e symbol from this state.
//
// @param symbol
//  The symbol to find a transition for.
//
// @return
//  The transition or null if there is no transition on \e symbol from this
//  state.
*/
GrammarTransition* GrammarState::find_transition_by_symbol( const GrammarSymbol* symbol )
{    
    GrammarTransition* transition = NULL;  
      
    if ( symbol )
    {
        std::set<GrammarTransition>::iterator i = transitions_.begin();
        while ( i != transitions_.end() && !i->taken_on_symbol(symbol) )
        {
            ++i;
        }
        transition = i != transitions_.end() ? const_cast<GrammarTransition*>(&(*i)) : NULL;
    }    
    
    return transition;
}

/**
// Generate indices for the transitions in this state.
*/
void GrammarState::generate_indices_for_transitions()
{
    int index = 0;
    for ( std::set<GrammarTransition>::iterator transition = transitions_.begin(); transition != transitions_.end(); ++transition )
    {
        transition->set_index( index );
        ++index;
    }
}

/**
// Set this state as having been processed.
//
// @param processed
//  True to mark this state as processed or false to mark it as not processed.
*/
void GrammarState::set_processed( bool processed )
{
    processed_ = processed;
}

/**
// Set the index of this state.
//
// @param index
//  The value to set the index of this state to.
*/
void GrammarState::set_index( int index )
{
    index_ = index;
}
