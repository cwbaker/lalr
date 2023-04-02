//
// GrammarState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarState.hpp"
#include "GrammarItem.hpp"
#include "GrammarTransition.hpp"
#include "GrammarSymbol.hpp"
#include "assert.hpp"
#include <stdio.h>

using std::set;
using std::string;
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

GrammarState::GrammarState( GrammarState&& state )
: items_()
, transitions_()
, processed_( false )
, index_( INVALID_INDEX )
{
    std::swap( items_, state.items_ );
    std::swap( transitions_, state.transitions_ );
    std::swap( processed_, state.processed_ );
    std::swap( index_, state.index_ );
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
    if ( symbol )
    {
        int index = symbol->index();
        if ( index >= 0 && index < int(transitions_.size()) )
        {
            const GrammarTransition* transition = transitions_[index];
            LALR_ASSERT( !transition || transition->symbol() == symbol );
            return transition;
        }
    }
    return nullptr;
}

/**
// Get the transitions from this state.
//
// @return
//  The transitions.
*/
const std::vector<GrammarTransition*>& GrammarState::transitions() const
{
    return transitions_;
}

int GrammarState::count_valid_transitions() const
{
    int valid_transitions = 0;
    for ( const GrammarTransition* transition : transitions_ )
    {
        valid_transitions += transition != nullptr;
    }
    return valid_transitions;
}

std::string GrammarState::label() const
{
    string label;
    for ( const GrammarItem& item : items_ )
    {
        if ( !label.empty() )
        {
            label += "\\n";
        }
        label += item.label();
    }
    return label;
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
void GrammarState::add_shift_transition( GrammarTransition* transition )
{
    LALR_ASSERT( transition );
    LALR_ASSERT( transition->is_shift() );
    LALR_ASSERT( !find_transition_by_symbol(transition->symbol()) );
    add_transition( transition );
}

/**
// Add a reduction to \e production from this state on \e symbol.
//
// @param symbol
//  The symbol to make the reduction on.
//
// @param production
//  The production to reduce.
*/
void GrammarState::add_reduce_transition( GrammarTransition* transition )
{
    LALR_ASSERT( transition );
    LALR_ASSERT( transition->is_reduce() );
    GrammarTransition* existing_transition = find_transition_by_symbol( transition->symbol() );
    if ( existing_transition )
    {
        existing_transition->override_shift_to_reduce( transition->production() );
    }
    else
    {
        add_transition( transition );
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
    if ( symbol )
    {
        int index = symbol->index();
        if ( index >= 0 && index < int(transitions_.size()) )
        {
            return transitions_[index];
        }
    }
    return nullptr;
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

void GrammarState::add_transition( GrammarTransition* transition )
{
    LALR_ASSERT( transition );
    LALR_ASSERT( transition->symbol() );
    int index = transition->symbol()->index();
    if ( index >= int(transitions_.size()) )
    {
        int transitions_to_insert = index - int(transitions_.size()) + 1;
        transitions_.insert( transitions_.end(), transitions_to_insert, nullptr );
    }
    LALR_ASSERT( index < int(transitions_.size()) );
    LALR_ASSERT( !transitions_[index] );
    transitions_[index] = transition;
}
