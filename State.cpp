//
// State.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "State.hpp"
#include "Item.hpp"
#include "Transition.hpp"
#include "assert.hpp"
#include <stdio.h>

using namespace sweet;
using namespace sweet::lalr;

using std::set;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
*/
State::State()
: items_(),
  transitions_(),
  processed_( false ),
  index_( INVALID_INDEX )
{
}

/**
// Get the items in this state.
//
// @return
//  The items.
*/
const std::set<Item>& State::items() const
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
const Transition* State::find_transition_by_symbol( const Symbol* symbol ) const
{    
    const Transition* transition = NULL;
    
    if ( symbol )
    {
        std::set<Transition>::const_iterator i = transitions_.begin();
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
const std::set<Transition>& State::transitions() const
{
    return transitions_;
}

/**
// Has this state been processed?
//
// @return
//  True if this state has been processed otherwise false.
*/
bool State::processed() const
{
    return processed_;
}

/**
// Get the index of this state.
//
// @return
//  The index of this state.
*/
int State::index() const
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
bool State::operator<( const State& state ) const
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
int State::add_item( Production* production, int position )
{
    SWEET_ASSERT( production );
    return items_.insert( Item(production, position) ).second ? 1 : 0;
}

/**
// Add the symbols in \e lookahead_symbols to the item in this state for 
// \e production at \e next_nodes.
//
// @param production
//  The production of the item to add \e lookahead_symbols to.
//
// @param position
//  The position of the dot in the item to add \e lookahead_symbols to.
//
// @param lookahead_symbols
//  The lookahead symbols to add to the item in this state.
//
// @return
//  The number of lookahead symbols added.
*/
int State::add_lookahead_symbols( Production* production, int position, const std::set<const Symbol*>& lookahead_symbols )
{
    SWEET_ASSERT( production );
    std::set<Item>::iterator item = items_.find( Item(production, position) );
    SWEET_ASSERT( item != items_.end() );
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
void State::add_transition( const Symbol* symbol, State* state )
{
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( state );
    SWEET_ASSERT( transitions_.find(Transition(symbol, state)) == transitions_.end() );
    transitions_.insert( Transition(symbol, state) );
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
void State::add_transition( const Symbol* symbol, const Symbol* reduced_symbol, int reduced_length, int precedence, int action )
{
    SWEET_ASSERT( symbol );
    SWEET_ASSERT( reduced_symbol );
    SWEET_ASSERT( reduced_length >= 0 );
    SWEET_ASSERT( precedence >= 0 );

    std::set<Transition>::iterator transition = transitions_.find( Transition(symbol, reduced_symbol, reduced_length, precedence, action) );
    if ( transition != transitions_.end() )
    {        
        SWEET_ASSERT( transition->type() == TRANSITION_SHIFT );
        transition->override_shift_to_reduce( reduced_symbol, reduced_length, precedence, action );
    }
    else
    {
        transition = transitions_.insert( Transition(symbol, reduced_symbol, reduced_length, precedence, action) ).first;
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
void State::add_transition( const std::set<const Symbol*>& symbols, const Symbol* reduced_symbol, int reduced_length, int precedence, int action )
{
    SWEET_ASSERT( reduced_symbol );
    SWEET_ASSERT( reduced_length >= 0 );
    SWEET_ASSERT( precedence >= 0 );
    for ( set<const Symbol*>::const_iterator i = symbols.begin(); i != symbols.end(); ++i )
    {
        const Symbol* symbol = *i;
        SWEET_ASSERT( symbol );
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
Transition* State::find_transition_by_symbol( const Symbol* symbol )
{    
    Transition* transition = NULL;  
      
    if ( symbol )
    {
        std::set<Transition>::iterator i = transitions_.begin();
        while ( i != transitions_.end() && !i->taken_on_symbol(symbol) )
        {
            ++i;
        }
        transition = i != transitions_.end() ? const_cast<Transition*>(&(*i)) : NULL;
    }    
    
    return transition;
}

/**
// Generate indices for the transitions in this state.
*/
void State::generate_indices_for_transitions()
{
    int index = 0;
    for ( std::set<Transition>::iterator transition = transitions_.begin(); transition != transitions_.end(); ++transition )
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
void State::set_processed( bool processed )
{
    processed_ = processed;
}

/**
// Set the index of this state.
//
// @param index
//  The value to set the index of this state to.
*/
void State::set_index( int index )
{
    index_ = index;
}
