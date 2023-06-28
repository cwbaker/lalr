#ifndef LALR_REGEXTRANSITION_HPP_INCLUDED
#define LALR_REGEXTRANSITION_HPP_INCLUDED

#include "assert.hpp"

namespace lalr
{

class RegexAction;
class RegexState;

/**
// A transition in a lexical analyzer's state machine.
*/
class RegexTransition
{
    int begin_; ///< The first character that the transition can be made on.
    int end_; ///< One past the last character that the transition can be made on.
    const RegexState* state_; ///< The state that is transitioned to.
    const RegexAction* action_; ///< The action that is taken on the transition or null if no action is taken.

public:
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
    RegexTransition( int begin, int end, const RegexState* state, const RegexAction* action )
    : begin_( begin )
    , end_( end )
    , state_( state )
    , action_( action )
    {
        LALR_ASSERT( begin_ < end_ );
        LALR_ASSERT( state_ );
    }

    int begin() const {return begin_;}
    int end() const {return end_;}
    bool on_character( int character ) const {return character >= begin_ && character < end_;}
    const RegexState* state() const
    {
        LALR_ASSERT( state_ );
        return state_;
    }
    const RegexAction* action() const {return action_;}
/**
// Less than operator.
//
// @return
//  True if both the beginning and end of this transition's interval is less
//  than the beginning of \e transition's interval.
*/
    bool operator<( const RegexTransition& transition ) const
    {
        return begin_ < transition.begin_ && end_ <= transition.begin_;
    }

};

}

#endif
