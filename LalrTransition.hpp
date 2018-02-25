#ifndef SWEET_LALR_LALRTRANSITION_HPP_INCLUDED
#define SWEET_LALR_LALRTRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class LalrState;
class LalrSymbol;

/**
// A transition in a parser's state machine.
*/
class LalrTransition
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        const LalrSymbol* symbol_; ///< The symbol that the transition is taken on.
        mutable LalrState* state_; ///< The state that is transitioned to.
        mutable const LalrSymbol* reduced_symbol_; ///< The symbol that is reduced to or null if this isn't a reducing transition.
        mutable int reduced_length_; ///< The number of symbols on the right-hand side of the reduced production.
        mutable int precedence_; ///< The precedence of the reduce production or 0 for the default precedence or no reduction.
        mutable int action_; ///< The index of the action taken on reduce or Action::INVALID_INDEX if there is no action associated with the reduce.
        mutable TransitionType type_; ///< The type of transition that this is.
        mutable int index_; ///< The index of this transition.

    public:
        LalrTransition( const LalrSymbol* symbol, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        LalrTransition( const LalrSymbol* symbol, LalrState* state );

        TransitionType get_type() const;
        LalrState* get_state() const;
        const LalrSymbol* reduced_symbol() const;
        int reduced_length() const;
        int precedence() const;
        int action() const;
        bool is_symbol( const LalrSymbol* symbol ) const;
        const LalrSymbol* get_symbol() const;
        void describe( std::string* description ) const;
        
        void set_index( int index ) const;
        int get_index() const;
        
        void override_shift_to_reduce( const LalrSymbol* symbol, int length, int precedence, int action ) const;
        void override_reduce_to_reduce( const LalrSymbol* symbol, int length, int precedence, int action ) const;
        
        bool operator<( const LalrTransition& transition ) const;
};

}

}

#endif
