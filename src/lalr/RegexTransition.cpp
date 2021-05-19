//
// RegexTransition.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexTransition.hpp"
#include "RegexAction.hpp"
#include "RegexState.hpp"
#include "assert.hpp"

using namespace lalr;

/**
// Constructor.
//
// @param begin
//  The first character in the interval that this transition can be taken on.
//
// @param end
//  One past the last character in the interval that this transition can be 
//  taken on.
//
// @param state
//  The state that is transitioned to (assumed not null).
//
// @param action
//  The action to take when this transition is taken or null if this 
//  transition doesn't take an action.
*/
RegexTransition::RegexTransition( int begin, int end, const RegexState* state, const RegexAction* action )
: begin_( begin )
, end_( end )
, state_( state )
, action_( action )
{
    LALR_ASSERT( begin_ < end_ );
    LALR_ASSERT( state_ );
}

/**
// Is this transition taken on \e character?
//
// @return
//  True if this transition is taken on character otherwise false.
*/
bool RegexTransition::on_character( int character ) const
{
    return character >= begin_ && character < end_;
}

/**
// Get the first character in the interval that this transition can be taken 
// on.
//
// @return
//  The first character.
*/
int RegexTransition::begin() const
{
    return begin_;
}

/**
// Get the character that is one past the last character in the interval 
// that this transition can be taken on.
//
// @return
//  The last character.
*/
int RegexTransition::end() const
{
    return end_;
}

/**
// Get the action that is taken when this transition is taken.
//
// @return
//  The action or null if this transition doesn't have an action.
*/
const RegexAction* RegexTransition::action() const
{
    return action_;
}

/**
// Get the state that this transition is to.
//
// @return
//  The state.
*/
const RegexState* RegexTransition::state() const
{
    LALR_ASSERT( state_ );
    return state_;
}

/**
// Less than operator.
//
// @return
//  True if both the beginning and end of this transition's interval is less
//  than the beginning of \e transition's interval.
*/
bool RegexTransition::operator<( const RegexTransition& transition ) const
{
    return begin_ < transition.begin_ && end_ <= transition.begin_;
}
