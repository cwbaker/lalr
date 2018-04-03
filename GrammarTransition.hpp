#ifndef LALR_GRAMMARTRANSITION_HPP_INCLUDED
#define LALR_GRAMMARTRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"
#include <string>
#include <set>

namespace lalr
{

class GrammarState;
class GrammarSymbol;

/**
// A transition in a parser's state machine.
*/
class GrammarTransition
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        const GrammarSymbol* symbol_; ///< The symbol that the transition is taken on.
        mutable GrammarState* state_; ///< The state that is transitioned to.
        mutable const GrammarSymbol* reduced_symbol_; ///< The symbol that is reduced to or null if this isn't a reducing transition.
        mutable int reduced_length_; ///< The number of symbols on the right-hand side of the reduced production.
        mutable int precedence_; ///< The precedence of the reduce production or 0 for the default precedence or no reduction.
        mutable int action_; ///< The index of the action taken on reduce or Action::INVALID_INDEX if there is no action associated with the reduce.
        mutable TransitionType type_; ///< The type of transition that this is.
        mutable int index_; ///< The index of this transition.

    public:
        GrammarTransition( const GrammarSymbol* symbol, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        GrammarTransition( const GrammarSymbol* symbol, GrammarState* state );
        TransitionType type() const;
        GrammarState* state() const;
        const GrammarSymbol* reduced_symbol() const;
        int reduced_length() const;
        int precedence() const;
        int action() const;
        bool taken_on_symbol( const GrammarSymbol* symbol ) const;
        const GrammarSymbol* symbol() const;
        int index() const;
        bool operator<( const GrammarTransition& transition ) const;

        void set_index( int index ) const;
        void override_shift_to_reduce( const GrammarSymbol* symbol, int length, int precedence, int action ) const;
        void override_reduce_to_reduce( const GrammarSymbol* symbol, int length, int precedence, int action ) const;
};

}

#endif
