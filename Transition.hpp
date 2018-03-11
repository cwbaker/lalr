#ifndef SWEET_LALR_TRANSITION_HPP_INCLUDED
#define SWEET_LALR_TRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class State;
class Symbol;

/**
// A transition in a parser's state machine.
*/
class Transition
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        const Symbol* symbol_; ///< The symbol that the transition is taken on.
        mutable State* state_; ///< The state that is transitioned to.
        mutable const Symbol* reduced_symbol_; ///< The symbol that is reduced to or null if this isn't a reducing transition.
        mutable int reduced_length_; ///< The number of symbols on the right-hand side of the reduced production.
        mutable int precedence_; ///< The precedence of the reduce production or 0 for the default precedence or no reduction.
        mutable int action_; ///< The index of the action taken on reduce or Action::INVALID_INDEX if there is no action associated with the reduce.
        mutable TransitionType type_; ///< The type of transition that this is.
        mutable int index_; ///< The index of this transition.

    public:
        Transition( const Symbol* symbol, const Symbol* reduced_symbol, int reduced_length, int precedence, int action );
        Transition( const Symbol* symbol, State* state );
        TransitionType type() const;
        State* state() const;
        const Symbol* reduced_symbol() const;
        int reduced_length() const;
        int precedence() const;
        int action() const;
        bool taken_on_symbol( const Symbol* symbol ) const;
        const Symbol* symbol() const;
        int index() const;
        bool operator<( const Transition& transition ) const;

        void set_index( int index ) const;
        void override_shift_to_reduce( const Symbol* symbol, int length, int precedence, int action ) const;
        void override_reduce_to_reduce( const Symbol* symbol, int length, int precedence, int action ) const;
};

}

}

#endif
