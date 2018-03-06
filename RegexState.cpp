//
// LexerState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "LexerState.hpp"
#include "LexerItem.hpp"
#include "LexerTransition.hpp"
#include "assert.hpp"
#include <stdio.h>

using namespace sweet;
using namespace sweet::lalr;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
*/
LexerState::LexerState()
: items_(),
  transitions_(),
  symbol_( NULL ),
  processed_( false ),
  index_( -1 )
{
}

/**
// Add an item to this state.
//
// If any of the nodes in \e next_nodes are at the end of their productions 
// then this state becomes an accepting state for the symbol that the node
// matches.
//
// @param next_nodes
//  The nodes that appear after the dot in the item to add.
//
// @return
//  The number of items added (0 or 1).
*/
int LexerState::add_item( const std::set<RegexNode*, RegexNodeLess>& next_nodes )
{
    return items_.insert( LexerItem(next_nodes) ).second ? 1 : 0;
}

/**
// Get the items that make up this state.
//
// @return
//  The items that make up this state.
*/
const std::set<LexerItem>& LexerState::get_items() const
{
    return items_;
}

/**
// Add a transition from this state to \e state on \e symbol.
//
// @param state
//  The state to add a transition to (assumed not null).
//
// @param begin
//  The begin character in the range to transition on.
//
// @param end
//  The end character in the range to transition on.
*/
void LexerState::add_transition( int begin, int end, LexerState* state )
{
    const LexerAction* action = NULL;
    std::set<LexerItem>::const_iterator item = items_.begin();
    while ( item != items_.end() && !action )
    {
        action = item->find_action_by_interval( begin, end );
        ++item;
    }

    bool inserted = transitions_.insert( LexerTransition(begin, end, state, action) ).second;
    SWEET_ASSERT( inserted );
    (void) inserted;
}

/**
// Find the transition from this state on \e character.
//
// @param character
//  The character to find a transition from this state on.
//
// @return
//  The transition to make on \e character or null if there is no transition 
//  from this state on \e character.
*/
const LexerTransition* LexerState::find_transition_by_character( int character ) const
{
    std::set<LexerTransition>::const_iterator transition = transitions_.begin();
    while ( transition != transitions_.end() && !transition->is_on_character(character) )
    {
        ++transition;
    }

    return transition != transitions_.end() ? &(*transition) : NULL;
}

/**
// Get the transitions from this state.
//
// @return
//  The transitions from this state.
*/
const std::set<LexerTransition>& LexerState::get_transitions() const
{
    return transitions_;
}

/**
// Set the symbol that this state matches.
//
// @param symbol
//  The symbol to set this state as matching (assumed not null).
*/
void LexerState::set_symbol( const void* symbol )
{
    SWEET_ASSERT( !symbol_ );
    symbol_ = symbol;
}

/**
// Get the symbol that this state matches.
//
// @return
//  The symbol that this state matches or null if this state doesn't match a
//  symbol.
*/
const void* LexerState::get_symbol() const
{
    return symbol_;
}

/**
// Set whether or not this state has been processed.
//
// @param processed
//  True to set this state as processed.
*/
void LexerState::set_processed( bool processed )
{
    processed_ = processed;
}

/**
// Has this state been processed?
//
// @return
//  True if this state has been processed otherwise false.
*/
bool LexerState::is_processed() const
{
    return processed_;
}

/**
// Set the index of this state.
//
// @param index
//  The value to set the index of this state to.
*/
void LexerState::set_index( int index )
{
    index_ = index;
}

/**
// Get the index of this state.
//
// @return
//  The index of this state.
*/
int LexerState::get_index() const
{
    return index_;
}

/**
// Less than operator.
//
// @return
//  True if this items of this state are lexically less than the items of 
//  \e state.
*/
bool LexerState::operator<( const LexerState& state ) const
{
    return std::lexicographical_compare( items_.begin(), items_.end(), state.items_.begin(), state.items_.end() );
}

/**
// Describe this state.
//
// @param description
//  A variable to append the description of this state to (assumed not null).
*/
void LexerState::describe( std::string* description ) const
{
    SWEET_ASSERT( description );

    char buffer [512];
    snprintf( buffer, sizeof(buffer), "%d (%p):\n", index_, symbol_ );
    buffer [sizeof(buffer) - 1] = '\0';
    description->append( buffer );

    std::set<LexerItem>::const_iterator item = items_.begin(); 
    while ( item != items_.end() )
    {
        item->describe( description );
        description->append( "\n" );
        ++item;
    }

    std::set<LexerTransition>::const_iterator transition = transitions_.begin();
    while ( transition != transitions_.end() )
    {
        transition->describe( description );
        description->append( "\n" );
        ++transition;
    }
}
