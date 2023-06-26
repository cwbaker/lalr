//
// GrammarState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "GrammarState.hpp"
#include "GrammarItem.hpp"
#include "GrammarTransition.hpp"
#include "GrammarSymbol.hpp"
#include "GrammarProduction.hpp"
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
, transitions_by_symbol_index_()
, transitions_()
, index_( INVALID_INDEX )
{
}

GrammarState::GrammarState( GrammarState&& state )
: items_()
, transitions_by_symbol_index_()
, transitions_()
, index_( INVALID_INDEX )
{
    std::swap( items_, state.items_ );
    std::swap( transitions_by_symbol_index_, state.transitions_by_symbol_index_ );
    std::swap( transitions_, state.transitions_ );
    std::swap( index_, state.index_ );
}

GrammarState::GrammarState( const GrammarState& state )
: items_( state.items_ )
, transitions_by_symbol_index_( state.transitions_by_symbol_index_ )
, transitions_( state.transitions_ )
, index_( state.index_ )
{
}

/**
// Find the item for *production* at *position* in this state.
//
// @param production
//  The production to find the item for, assumed not null.
//
// @param position
//  The position to find the item for, assumed zero or greater.
//
// @return
//  The matching item or null if no such item was found.
*/
GrammarItem* GrammarState::find_item( GrammarProduction* production, int position ) const
{
    LALR_ASSERT( production );
    LALR_ASSERT( position >= 0 && position <= production->length() );
    set<GrammarItem>::iterator i = items_.find( GrammarItem(production, position) );
    return i != items_.end() ? const_cast<GrammarItem*>(&(*i)) : nullptr;
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
        if ( index >= 0 && index < int(transitions_by_symbol_index_.size()) )
        {
            const GrammarTransition* transition = transitions_by_symbol_index_[index];
            LALR_ASSERT( !transition || transition->symbol() == symbol );
            return transition;
        }
    }
    return nullptr;
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
        if ( index >= 0 && index < int(transitions_by_symbol_index_.size()) )
        {
            GrammarTransition* transition = transitions_by_symbol_index_[index];
            LALR_ASSERT( !transition || transition->symbol() == symbol );
            return transition;
        }
    }
    return nullptr;
}

void GrammarState::add_transition( GrammarTransition* transition )
{
    LALR_ASSERT( transition );
    LALR_ASSERT( transition->symbol() );
    int index = transition->symbol()->index();
    if ( index >= int(transitions_by_symbol_index_.size()) )
    {
        int transitions_to_insert = index - int(transitions_by_symbol_index_.size()) + 1;
        transitions_by_symbol_index_.insert( transitions_by_symbol_index_.end(), transitions_to_insert, nullptr );
    }
    LALR_ASSERT( index < int(transitions_by_symbol_index_.size()) );
    LALR_ASSERT( !transitions_by_symbol_index_[index] );
    transitions_by_symbol_index_[index] = transition;
    transitions_.push_back( transition );
}
