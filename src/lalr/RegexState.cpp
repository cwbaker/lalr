//
// RegexState.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexState.hpp"
#include "RegexItem.hpp"
#include "RegexTransition.hpp"
#include "assert.hpp"
#include <stdio.h>

using namespace lalr;

#if defined(BUILD_PLATFORM_MSVC)
#define snprintf _snprintf
#endif

/**
// Constructor.
*/
RegexState::RegexState()
: items_()
, transitions_()
, symbol_( nullptr )
, processed_( false )
, index_( -1 )
{
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
const RegexTransition* RegexState::find_transition_by_character( int character ) const
{
    std::set<RegexTransition>::const_iterator transition = transitions_.begin();
    while ( transition != transitions_.end() && !transition->on_character(character) )
    {
        ++transition;
    }

    return transition != transitions_.end() ? &(*transition) : nullptr;
}

/**
// Less than operator.
//
// @return
//  True if this items of this state are lexically less than the items of 
//  \e state.
*/
bool RegexState::operator<( const RegexState& state ) const
{
    return std::lexicographical_compare( items_.begin(), items_.end(), state.items_.begin(), state.items_.end() );
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
int RegexState::add_item( const std::set<RegexNode*, RegexNodeLess>& next_nodes )
{
    return items_.insert( RegexItem(next_nodes) ).second ? 1 : 0;
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
void RegexState::add_transition( int begin, int end, RegexState* state )
{
    const RegexAction* action = nullptr;
    std::set<RegexItem>::const_iterator item = items_.begin();
    while ( item != items_.end() && !action )
    {
        action = item->find_action_by_interval( begin, end );
        ++item;
    }

    bool inserted = transitions_.insert( RegexTransition(begin, end, state, action) ).second;
    LALR_ASSERT( inserted );
    (void) inserted;
}
